
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

    // std::string instanceName{"image-classification-inference"};
    std::string instanceName{"face_analyser"};
    // std::string modelFilepath{"../../data/models/squeezenet1.1-7.onnx"};
    // std::string modelFilepath{"../../data/models/resnet18-v1-7.onnx"};
    std::string modelFilepath{"../../data/models/yunet.onnx"};
    // std::string modelFilepath{"../../data/models/retinaface_10g.onnx"};
    std::string imageFilepath{
        // "../../data/images/european-bee-eater-2115564_1920.jpg"};
        "../../data/images/face1.jpg"};
    // std::string labelFilepath{"../../data/labels/synset.txt"};  

    // std::vector<std::string> labels{readLabels(labelFilepath)}; //读取类别标签

    Ort::Env env(OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING,
                 instanceName.c_str());
    Ort::SessionOptions sessionOptions;
    sessionOptions.SetIntraOpNumThreads(1); //指定一个线程运行
    if (useCUDA)
    {
        // Using CUDA backend
        // https://github.com/microsoft/onnxruntime/blob/v1.8.2/include/onnxruntime/core/session/onnxruntime_cxx_api.h#L329
        OrtCUDAProviderOptions cuda_options{};
        sessionOptions.AppendExecutionProvider_CUDA(cuda_options);
    }

    // Sets graph optimization level
    // Available levels are
    // ORT_DISABLE_ALL -> To disable all optimizations
    // ORT_ENABLE_BASIC -> To enable basic optimizations (Such as redundant node
    // removals) ORT_ENABLE_EXTENDED -> To enable extended optimizations
    // (Includes level 1 + more complex optimizations like node fusions)
    // ORT_ENABLE_ALL -> To Enable All possible optimizations
    sessionOptions.SetGraphOptimizationLevel(
        GraphOptimizationLevel::ORT_ENABLE_EXTENDED);   //设置图优化级别

    Ort::Session session(env, modelFilepath.c_str(), sessionOptions);   //创建会话

    Ort::AllocatorWithDefaultOptions allocator;

    //获取输入、输出结点数
    size_t numInputNodes = session.GetInputCount(); 
    size_t numOutputNodes = session.GetOutputCount();

    // const char* inputName = session.GetInputName(0, allocator);
    const char* inputName = strdup(session.GetInputNameAllocated(0, allocator).get());
    std::cout << inputName << std::endl; //测试乱码

    Ort::TypeInfo inputTypeInfo = session.GetInputTypeInfo(0);
    auto inputTensorInfo = inputTypeInfo.GetTensorTypeAndShapeInfo();

    ONNXTensorElementDataType inputType = inputTensorInfo.GetElementType();

    std::vector<int64_t> inputDims = inputTensorInfo.GetShape();

    if (inputDims.at(0) == -1)  //接收动态批处理大小
    {
        std::cout << "Got dynamic batch size. Setting input batch size to "
                  << batchSize << "." << std::endl;
        inputDims.at(0) = batchSize;
    }

    // const char* outputName = session.GetOutputName(0, allocator);
    const char* outputName = strdup(session.GetOutputNameAllocated(0, allocator).get());
    const char* outputName1 = strdup(session.GetOutputNameAllocated(1, allocator).get());
    const char* outputName2 = strdup(session.GetOutputNameAllocated(2, allocator).get());


    Ort::TypeInfo outputTypeInfo = session.GetOutputTypeInfo(0);
    auto outputTensorInfo = outputTypeInfo.GetTensorTypeAndShapeInfo();

    Ort::TypeInfo outputTypeInfo1 = session.GetOutputTypeInfo(1);
    auto outputTensorInfo1 = outputTypeInfo1.GetTensorTypeAndShapeInfo();

    Ort::TypeInfo outputTypeInfo2 = session.GetOutputTypeInfo(2);
    auto outputTensorInfo2 = outputTypeInfo2.GetTensorTypeAndShapeInfo();


    ONNXTensorElementDataType outputType = outputTensorInfo.GetElementType();
    ONNXTensorElementDataType outputType1 = outputTensorInfo1.GetElementType();
    ONNXTensorElementDataType outputType2 = outputTensorInfo2.GetElementType();

    std::vector<int64_t> outputDims = outputTensorInfo.GetShape();
    std::vector<int64_t> outputDims1 = outputTensorInfo1.GetShape();
    std::vector<int64_t> outputDims2 = outputTensorInfo2.GetShape();
    if (outputDims.at(0) == -1)
    {
        std::cout << "Got dynamic batch size. Setting output batch size to "
                  << batchSize << "." << std::endl;
        outputDims.at(0) = batchSize;
    }
    outputDims1.at(0) = batchSize;
    outputDims2.at(0) = batchSize;

    //retinaface、yunet设置输入的宽高
    inputDims.at(3) = 224;
    inputDims.at(2) = 224;

    std::cout << "Number of Input Nodes: " << numInputNodes << std::endl;
    std::cout << "Number of Output Nodes: " << numOutputNodes << std::endl;
    std::cout << "Input Name: " << inputName << std::endl;
    std::cout << "Input Type: " << inputType << std::endl;
    std::cout << "Input Dimensions: " << inputDims << std::endl;
    std::cout << "Output Name: " << outputName << std::endl;
    std::cout << "Output Name: " << outputName1 << std::endl;
    std::cout << "Output Name: " << outputName2 << std::endl;

    std::cout << "Output Type: " << outputType << std::endl;
    std::cout << "Output Type: " << outputType1 << std::endl;
    std::cout << "Output Type: " << outputType2 << std::endl;
    
    std::cout << "Output Dimensions: " << outputDims << std::endl;
    std::cout << "Output Dimensions: " << outputDims1 << std::endl;
    std::cout << "Output Dimensions: " << outputDims2 << std::endl;
    
    //读取图片
    cv::Mat imageBGR = cv::imread(imageFilepath, cv::ImreadModes::IMREAD_COLOR);
    cv::Mat resizedImageBGR, resizedImageRGB, resizedImage, preprocessedImage;


    cv::resize(imageBGR, resizedImageBGR,
               cv::Size(inputDims.at(3), inputDims.at(2)),
               cv::InterpolationFlags::INTER_CUBIC);    //重构图像形状，利用三次方差值方法获得更平滑的图像
    
    // //指定缩放后的大小
    // int width = 640;  // 目标宽度
    // int height = 640; // 目标高度，确保这是正值
    // cv::resize(imageBGR, resizedImageBGR,
    //            cv::Size(width, height),
    //            cv::InterpolationFlags::INTER_CUBIC);    //重构图像形状，利用三次方差值方法获得更平滑的图像

    cv::cvtColor(resizedImageBGR, resizedImageRGB,
                 cv::ColorConversionCodes::COLOR_BGR2RGB);
    resizedImageRGB.convertTo(resizedImage, CV_32F, 1.0 / 255); //用32位浮点数归一化为1-255

    cv::Mat channels[3];
    cv::split(resizedImage, channels);  //通道分离
    // Normalization per channel
    // Normalization parameters obtained from
    // https://github.com/onnx/models/tree/master/vision/classification/squeezenet
    channels[0] = (channels[0] - 0.485) / 0.229;
    channels[1] = (channels[1] - 0.456) / 0.224;
    channels[2] = (channels[2] - 0.406) / 0.225;
    cv::merge(channels, 3, resizedImage);
    // HWC to CHW
    cv::dnn::blobFromImage(resizedImage, preprocessedImage);

    size_t inputTensorSize = vectorProduct(inputDims);  //计算整个输入张量的总元素数量
    std::vector<float> inputTensorValues(inputTensorSize);
    // Make copies of the same image input.
    for (int64_t i = 0; i < batchSize; ++i)
    {
        std::copy(preprocessedImage.begin<float>(),
                  preprocessedImage.end<float>(),
                  inputTensorValues.begin() + i * inputTensorSize / batchSize); //只输入一张图片，batch为2，则相当于拷贝原图片，输出结果也是相同的
    }


    size_t outputTensorSize = vectorProduct(outputDims);
    size_t outputTensorSize1 = vectorProduct(outputDims1);
    size_t outputTensorSize2 = vectorProduct(outputDims2);

    assert(("Output tensor size should equal to the label set size.",
            labels.size() * batchSize == outputTensorSize));
    std::vector<float> outputTensorValues(outputTensorSize);
    std::vector<float> outputTensorValues1(outputTensorSize1);
    std::vector<float> outputTensorValues2(outputTensorSize2);

    std::vector<const char*> inputNames{inputName};
    // std::vector<const char*> outputNames{outputName};
    std::vector<const char*> outputNames;
    outputNames.push_back(outputName);
    outputNames.push_back(outputName1);
    outputNames.push_back(outputName2);
    std::vector<Ort::Value> inputTensors;
    std::vector<Ort::Value> outputTensors;

    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(
        OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault);
    inputTensors.push_back(Ort::Value::CreateTensor<float>(
        memoryInfo, inputTensorValues.data(), inputTensorSize, inputDims.data(),
        inputDims.size()));
        
    outputTensors.push_back(Ort::Value::CreateTensor<float>(
        memoryInfo, outputTensorValues.data(), outputTensorSize,
        outputDims.data(), outputDims.size()));

    outputTensors.push_back(Ort::Value::CreateTensor<float>(
        memoryInfo, outputTensorValues1.data(), outputTensorSize1,
        outputDims1.data(), outputDims1.size()));

    outputTensors.push_back(Ort::Value::CreateTensor<float>(
        memoryInfo, outputTensorValues2.data(), outputTensorSize2,
        outputDims2.data(), outputDims2.size()));
    //执行推理
    std::cout << outputNames.size() << std::endl;
    session.Run(Ort::RunOptions{nullptr}, inputNames.data(),
                inputTensors.data(), 1 /*Number of inputs*/, outputNames.data(),
                outputTensors.data(), 3 /*Number of outputs*/);






    // //类别标签处理
    // std::vector<int> predIds(batchSize, 0);
    // std::vector<std::string> predLabels(batchSize);
    // std::vector<float> confidences(batchSize, 0.0f);
    // for (int64_t b = 0; b < batchSize; ++b)
    // {
    //     float activation = 0;
    //     float maxActivation = std::numeric_limits<float>::lowest(); //最大激活值设置为float类型可能的最小值
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

    // ———————————————————— Measure latency ————————————————————————
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
