# ONNX Runtime Inference

## Introduction

(clh)ONNX Runtime C++ inference example for image classification using CPU and CUDA.

## Dependencies

* CMake 3.20.1
* ONNX Runtime 1.12.0
* OpenCV 4.5.2

## Usages

### Build Docker Image

```bash
$ docker build -f docker/onnxruntime-cuda.Dockerfile --no-cache --tag=onnxruntime-cuda:1.12.0 .
```

### Run Docker Container

```bash
$ docker run -it --rm --gpus device=0 -v $(pwd):/mnt onnxruntime-cuda:1.12.0
```

### Build Example

```bash
$ cmake -B build
$ cmake --build build --config Release --parallel
```

### Run Example

```bash
$ cd build/src/
$ ./inference  --use_cpu
Inference Execution Provider: CPU
Number of Input Nodes: 1
Number of Output Nodes: 1
Input Name: data
Input Type: float
Input Dimensions: [1, 3, 224, 224]
Output Name: squeezenet0_flatten0_reshape0
Output Type: float
Output Dimensions: [1, 1000]
Predicted Label ID: 92
Predicted Label: n01828970 bee eater
Uncalibrated Confidence: 0.996137
Minimum Inference Latency: 7.45 ms
```

```bash
$ cd build/src/
$ ./inference  --use_cuda
Inference Execution Provider: CUDA
Number of Input Nodes: 1
Number of Output Nodes: 1
Input Name: data
Input Type: float
Input Dimensions: [1, 3, 224, 224]
Output Name: squeezenet0_flatten0_reshape0
Output Type: float
Output Dimensions: [1, 1000]
Predicted Label ID: 92
Predicted Label: n01828970 bee eater
Uncalibrated Confidence: 0.996137
Minimum Inference Latency: 0.98 ms
```

## References

* [ONNX Runtime C++ Inference](https://leimao.github.io/blog/ONNX-Runtime-CPP-Inference/)

## Models Download

脸部分析和换脸模型下载可参考facefusion项目代码face_analyser.py和face_swapper.py
* [facefusion](https://github.com/facefusion/facefusion/tree/master)
