#include <Arduino.h>
#include <Wire.h>
#include <OV767X.h>

OV767X cam;

void setup() {
  Serial.begin(9600);
  cam.begin();
}

void loop() {
  if (cam.takePicture()) {
    cam.resize(160, 120);
    cam.toGrayScale();
    detectEdges();
  }
}

void detectEdges() {
  // Buffer to hold the grayscale image
  uint8_t grayImage[160 * 120];

  // Convert the image to grayscale
  cam.getPicture(grayImage);

  // Perform edge detection
  for (int y = 1; y < 120 - 1; y++) {
    for (int x = 1; x < 160 - 1; x++) {
      // Calculate horizontal and vertical gradients using Sobel operators
      int gradientX = grayImage[(y - 1) * 160 + (x + 1)] + 2 * grayImage[y * 160 + (x + 1)] + grayImage[(y + 1) * 160 + (x + 1)] -
                      grayImage[(y - 1) * 160 + (x - 1)] - 2 * grayImage[y * 160 + (x - 1)] - grayImage[(y + 1) * 160 + (x - 1)];
      int gradientY = grayImage[(y + 1) * 160 + (x - 1)] + 2 * grayImage[(y + 1) * 160 + x] + grayImage[(y + 1) * 160 + (x + 1)] -
                      grayImage[(y - 1) * 160 + (x - 1)] - 2 * grayImage[(y - 1) * 160 + x] - grayImage[(y - 1) * 160 + (x + 1)];

      // Calculate gradient magnitude
      int magnitude = abs(gradientX) + abs(gradientY);

      // Apply a threshold to detect edges
      if (magnitude > 100) {
        // Found an edge, do something (e.g., turn on an LED)
        digitalWrite(LED_BUILTIN, HIGH);
      } else {
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
  }
}
