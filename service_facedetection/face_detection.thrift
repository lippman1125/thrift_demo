struct DetectionInput {
    1: required binary img;	
}
struct DetectionResult {
    1: required list<list<double>> detections;
}
service FaceDetect {
  DetectionResult Detect(1: DetectionInput input)
}
