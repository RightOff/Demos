
// https://github.com/microsoft/onnxruntime/blob/v1.8.2/csharp/test/Microsoft.ML.OnnxRuntime.EndToEndTests.Capi/CXX_Api_Sample.cpp
// https://github.com/microsoft/onnxruntime/blob/v1.8.2/include/onnxruntime/core/session/onnxruntime_cxx_api.h
#include <onnxruntime_cxx_api.h>

#include <opencv2/dnn/dnn.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <chrono>
#include <cmath>
#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

template <typename T>
T vectorProduct(const std::vector<T>& v)
{
    return accumulate(v.begin(), v.end(), 1, std::multiplies<T>());
}

/**
 * @brief Operator overloading for printing vectors
 * @tparam T
 * @param os
 * @param v
 * @return std::ostream&
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << "[";
    for (int i = 0; i < v.size(); ++i)
    {
        os << v[i];
        if (i != v.size() - 1)
        {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

/**
 * @brief Print ONNX tensor data type
 * https://github.com/microsoft/onnxruntime/blob/rel-1.6.0/include/onnxruntime/core/session/onnxruntime_c_api.h#L93
 * @param os
 * @param type
 * @return std::ostream&
 */
std::ostream& operator<<(std::ostream& os,
                         const ONNXTensorElementDataType& type)
{
    switch (type)
    {
        case ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_UNDEFINED:
            os << "undefined";
            break;
        case ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT:
            os << "float";
            break;
        case ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT8:
            os << "uint8_t";
            break;
        case ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_INT8:
            os << "int8_t";
            break;
        case ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT16:
            os << "uint16_t";
            break;
        case ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_INT16:
            os << "int16_t";
            break;
        case ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_INT32:
            os << "int32_t";
            break;
        case ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64:
            os << "int64_t";
            break;
        case ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_STRING:
            os << "std::string";
            break;
        case ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_BOOL:
            os << "bool";
            break;
        case ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT16:
            os << "float16";
            break;
        case ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_DOUBLE:
            os << "double";
            break;
        case ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT32:
            os << "uint32_t";
            break;
        case ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT64:
            os << "uint64_t";
            break;
        case ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_COMPLEX64:
            os << "float real + float imaginary";
            break;
        case ONNXTensorElementDataType::
            ONNX_TENSOR_ELEMENT_DATA_TYPE_COMPLEX128:
            os << "double real + float imaginary";
            break;
        case ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_BFLOAT16:
            os << "bfloat16";
            break;
        default:
            break;
    }

    return os;
}

std::vector<std::string> readLabels(std::string& labelFilepath)
{
    std::vector<std::string> labels;
    std::string line;
    std::ifstream fp(labelFilepath);
    while (std::getline(fp, line))
    {
        labels.push_back(line);
    }
    return labels;
}

int main(int argc, char* argv[])
{
    const int64_t batchSize = 1;
    const int64_t modelInputSize = 640;
    const int64_t NumThreads = 4;
    bool useCUDA{true};
    const char* useCUDAFlag = "--use_cuda";
    const char* useCPUFlag = "--use_cpu";
    if (argc == 1)
    {
        useCUDA = false;
    }
    else if ((argc == 2) && (strcmp(argv[1], useCUDAFlag) == 0))
    {
        useCUDA = true;
    }
    else if ((argc == 2) && (strcmp(argv[1], useCPUFlag) == 0))
    {
        useCUDA = false;
    }
    else if ((argc == 2) && (strcmp(argv[1], useCUDAFlag) != 0))
    {
        useCUDA = false;
    }
    else
    {
        throw std::runtime_error{"Too many arguments."};
    }

    if (useCUDA)
    {
        std::cout << "Inference Execution Provider: CUDA" << std::endl;
    }
    else
    {
        std::cout << "Inference Execution Provider: CPU" << std::endl;
    }

    std::string instanceName{"face_analyser"};
    // std::string modelFilepath{"../../data/models/squeezenet1.1-7.onnx"};
    std::string modelFilepath{"../../data/models/retinaface_10g.onnx"};
    std::string imageFilepath{
        "../../data/images/european-bee-eater-2115564_1920.jpg"};
    // std::string labelFilepath{"../../data/labels/synset.txt"};

    // std::vector<std::string> labels{readLabels(labelFilepath)};

    Ort::Env env(OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING,
                 instanceName.c_str());
    Ort::SessionOptions sessionOptions;
    sessionOptions.SetIntraOpNumThreads(NumThreads);
    if (useCUDA)
    {
        // Using CUDA backend
        // https://github.com/microsoft/onnxruntime/blob/v1.8.2/include/onnxruntime/core/session/onnxruntime_cxx_api.h#L329
        OrtCUDAProviderOptions cuda_options{};
        sessionOptions.AppendExecutionProvider_CUDA(cuda_options);
        // OrtTensorRTProviderOptions trt_options{};
        // sessionOptions.AppendExecutionProvider_TensorRT(trt_options)
    }

    // Sets graph optimization level
    // Available levels are
    // ORT_DISABLE_ALL -> To disable all optimizations
    // ORT_ENABLE_BASIC -> To enable basic optimizations (Such as redundant node
    // removals) ORT_ENABLE_EXTENDED -> To enable extended optimizations
    // (Includes level 1 + more complex optimizations like node fusions)
    // ORT_ENABLE_ALL -> To Enable All possible optimizations
    //sessionOptions.SetGraphOptimizationLevel(
     //   GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

    Ort::Session session(env, modelFilepath.c_str(), sessionOptions);

    Ort::AllocatorWithDefaultOptions allocator;

    size_t numInputNodes = session.GetInputCount();
    size_t numOutputNodes = session.GetOutputCount();

    const char* inputName = session.GetInputName(0, allocator);
    // const char* inputName = strdup(session.GetInputNameAllocated(0, allocator).get());

    Ort::TypeInfo inputTypeInfo = session.GetInputTypeInfo(0);
    auto inputTensorInfo = inputTypeInfo.GetTensorTypeAndShapeInfo();

    ONNXTensorElementDataType inputType = inputTensorInfo.GetElementType();

    std::vector<int64_t> inputDims = inputTensorInfo.GetShape();
    if (inputDims.at(0) == -1)
    {
        std::cout << "Got dynamic batch size. Setting input batch size to "
                  << batchSize << "." << std::endl;
        inputDims.at(0) = batchSize;
    }
    if (inputDims.at(2) == -1)
    {
        inputDims.at(2) = modelInputSize;
        inputDims.at(3) = modelInputSize;
    }

    // const char* outputName = session.GetOutputName(0, allocator);
    // const char* outputName = strdup(session.GetOutputNameAllocated(0, allocator).get());

    //----------------------从模型中获取输出信息----------------------
    std::vector<const char*> outputNames;   //输出名字
    std::vector<ONNXTensorElementDataType> outputTypes; //输出类型信息
    std::vector<std::vector<int64_t>> outputDims(numOutputNodes);   //输出形状
    
    // 为输出名分配足够的空间
    outputNames.resize(numOutputNodes);  
    outputTypes.resize(numOutputNodes);  
    
    for (size_t i = 0; i < numOutputNodes; ++i) {
        // 将输出名称存储在vector中
        // outputNames[i] = strdup(session.GetOutputNameAllocated(i, allocator).get());
        outputNames[i] = session.GetOutputName(i, allocator);
        //获取类型信息
        Ort::TypeInfo outputTypeInfo = session.GetOutputTypeInfo(i);
        auto outputTensorInfo = outputTypeInfo.GetTensorTypeAndShapeInfo();
        outputTypes[i] = outputTensorInfo.GetElementType();

        //获取形状信息
        std::vector<int64_t> outputDim = outputTensorInfo.GetShape();
        if (outputDim.at(0) == -1)
        {
            // std::cout << "Got dynamic batch size. Setting output batch size to "
            //         << batchSize << "." << std::endl;
            outputDim.at(0) = batchSize;
        }
        outputDims[i] = outputDim;
    }


    std::cout << "Number of Input Nodes: " << numInputNodes << std::endl;
    std::cout << "Number of Output Nodes: " << numOutputNodes << std::endl;
    std::cout << "Input Name: " << inputName << std::endl;
    std::cout << "Input Type: " << inputType << std::endl;
    std::cout << "Input Dimensions: " << inputDims << std::endl;
    std::cout << "Output Name: " << outputNames << std::endl;
    std::cout << "Output Type: " << outputTypes << std::endl;
    std::cout << "Output Dimensions: " << outputDims << std::endl;

    //----------------------输入图片处理----------------------
    cv::Mat imageBGR = cv::imread(imageFilepath, cv::ImreadModes::IMREAD_COLOR);
    cv::Mat resizedImageBGR, resizedImageRGB, resizedImage, preprocessedImage;
    cv::resize(imageBGR, resizedImageBGR,
               cv::Size(inputDims.at(3), inputDims.at(2)),
               cv::InterpolationFlags::INTER_CUBIC);
    cv::cvtColor(resizedImageBGR, resizedImageRGB,
                 cv::ColorConversionCodes::COLOR_BGR2RGB);
    resizedImageRGB.convertTo(resizedImage, CV_32F, 1.0 / 255);

    cv::Mat channels[3];
    cv::split(resizedImage, channels);
    // Normalization per channel
    // Normalization parameters obtained from
    // https://github.com/onnx/models/tree/master/vision/classification/squeezenet
    channels[0] = (channels[0] - 0.485) / 0.229;
    channels[1] = (channels[1] - 0.456) / 0.224;
    channels[2] = (channels[2] - 0.406) / 0.225;
    cv::merge(channels, 3, resizedImage);
    // HWC to CHW
    cv::dnn::blobFromImage(resizedImage, preprocessedImage);

    //----------------------输入张量创建----------------------
    size_t inputTensorSize = vectorProduct(inputDims);
    std::vector<float> inputTensorValues(inputTensorSize);
    // Make copies of the same image input.
    for (int64_t i = 0; i < batchSize; ++i)
    {
        std::copy(preprocessedImage.begin<float>(),
                  preprocessedImage.end<float>(),
                  inputTensorValues.begin() + i * inputTensorSize / batchSize);
    }

    //----------------------输出张量创建----------------------
    std::vector<size_t> outputTensorSizes(numOutputNodes);
    std::vector<std::vector<float>> outputTensorValues(numOutputNodes);
    for (size_t i = 0; i < numOutputNodes; ++i){
        size_t outputTensorSize = vectorProduct(outputDims[i]);
        outputTensorSizes[i] = outputTensorSize;
        assert(("Output tensor size should equal to the label set size.",
            labels.size() * batchSize == outputTensorSize));
        std::vector<float> outputTensorValue(outputTensorSize);
        outputTensorValues[i] = outputTensorValue;
    }
    

    std::vector<const char*> inputNames{inputName};
    // std::vector<const char*> outputNames{outputName};
    std::vector<Ort::Value> inputTensors;
    std::vector<Ort::Value> outputTensors;

    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(
        OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault);
    inputTensors.push_back(Ort::Value::CreateTensor<float>(
        memoryInfo, inputTensorValues.data(), inputTensorSize, inputDims.data(),
        inputDims.size()));

    for(int64_t i = 0; i < numOutputNodes; ++i){
        outputTensors.push_back(Ort::Value::CreateTensor<float>(
        memoryInfo, outputTensorValues[i].data(), outputTensorSizes[i],
        outputDims[i].data(), outputDims[i].size()));
    }
    

    session.Run(Ort::RunOptions{nullptr}, inputNames.data(),
                inputTensors.data(), 1 /*Number of inputs*/, outputNames.data(),
                outputTensors.data(), 3 /*Number of outputs*/);

    // std::vector<int> predIds(batchSize, 0);
    // std::vector<std::string> predLabels(batchSize);
    // std::vector<float> confidences(batchSize, 0.0f);
    // for (int64_t b = 0; b < batchSize; ++b)
    // {
    //     float activation = 0;
    //     float maxActivation = std::numeric_limits<float>::lowest();
    //     float expSum = 0;
    //     for (int i = 0; i < labels.size(); i++)
    //     {
    //         activation = outputTensorValues.at(i + b * labels.size());
    //         expSum += std::exp(activation);
    //         if (activation > maxActivation)
    //         {
    //             predIds.at(b) = i;
    //             maxActivation = activation;
    //         }
    //     }
    //     predLabels.at(b) = labels.at(predIds.at(b));
    //     confidences.at(b) = std::exp(maxActivation) / expSum;
    // }
    // for (int64_t b = 0; b < batchSize; ++b)
    // {
    //     assert(("Output predictions should all be identical.",
    //             predIds.at(b) == predIds.at(0)));
    // }
    // // All the predictions should be the same
    // // because the input images are just copies of each other.

    // std::cout << "Predicted Label ID: " << predIds.at(0) << std::endl;
    // std::cout << "Predicted Label: " << predLabels.at(0) << std::endl;
    // std::cout << "Uncalibrated Confidence: " << confidences.at(0) << std::endl;

    //----------------------测试时延----------------------
    // Measure latency
    int numTests{100};
    std::chrono::steady_clock::time_point begin =
        std::chrono::steady_clock::now();
    for (int i = 0; i < numTests; i++)
    {
        session.Run(Ort::RunOptions{nullptr}, inputNames.data(),
                    inputTensors.data(), 1, outputNames.data(),
                    outputTensors.data(), 1);
    }
    std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now();
    std::cout << "Minimum Inference Latency: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                       begin)
                         .count() /
                     static_cast<float>(numTests)
              << " ms" << std::endl;
}