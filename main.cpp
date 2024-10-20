#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

bool drawing = false;
Point lastPoint;
Scalar selectedColor(0, 0, 0);

// Calculating the color by coordinates(x, y)
void getColorFromCoordinates(int x, int y) {

    int radius = 313;

    // Calculating the relative coordinates
    float X = x - 313;
    float Y = y - 313;

    // Calculating the distance from the center to the point
    float distance = std::sqrt(X * X + Y * Y);

    // Check if the point is inside the circle
    if (distance > radius) {
        selectedColor = Scalar(255,255,255);
    }

    // Calculating the angle to a range from 0 to 1
    const float PI = 3.14;
    float angle = (std::atan2(Y, X) + PI) / (2 * PI);

    // Converting the radius to a range from 0 to 1
    float R = distance / radius;

    // Calculating the color on the color wheel
    int hue = static_cast<int>(angle * 180); 

    // Converting HSV to BGR
    Mat hsv(1, 1, CV_8UC3);
    hsv.at<Vec3b>(0, 0)[0] = hue;                       // Angle
    hsv.at<Vec3b>(0, 0)[1] = static_cast<int>(255 * R); // Saturation
    hsv.at<Vec3b>(0, 0)[2] = 255;                       // Brightness

    Mat bgr;
    cvtColor(hsv, bgr, COLOR_HSV2BGR);
    selectedColor = Scalar(bgr.at<Vec3b>(0, 0)[0], bgr.at<Vec3b>(0, 0)[1], bgr.at<Vec3b>(0, 0)[2]);
}

// Drawing in photo
void mouseCallback(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        drawing = true;
        lastPoint = Point(x, y);
    }
    else if (event == EVENT_MOUSEMOVE && drawing) {
        Mat* img = reinterpret_cast<Mat*>(userdata);
        line(*img, lastPoint, Point(x, y), selectedColor, 2);
        lastPoint = Point(x, y);
    }
    else if (event == EVENT_LBUTTONUP) {
        drawing = false;
    }
}

// Selecting a color on the gradient color wheel
void mouseEvent(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        getColorFromCoordinates(x, y);
    }
}

int main() {
    Mat img = imread("nature.jpg");
    namedWindow("Image");

    Mat gcw = imread("gcw.png");
    namedWindow("GCW");

    // Slider for Brightness
    int SliderValue1 = 50;
    createTrackbar("Brightness", "Image", &SliderValue1, 100);

    // Slider for Contrast
    int SliderValue2 = 50;
    createTrackbar("Contrast", "Image", &SliderValue2, 100);

    Mat bin = img.clone();

    setMouseCallback("GCW", mouseEvent, &gcw);

    while (true) {

        Mat temp;
        int Brightness = SliderValue1 - 50;
        double Contrast = (SliderValue2 + 1) / 50.0;

        img.convertTo(temp, -1, Contrast, Brightness);

        setMouseCallback("Image", mouseCallback, &img);

        // Show the image
        imshow("Image", temp);
        imshow("GCW", gcw);

        int Key = waitKey(50);

        // If user presses 'C', image will be cleared
        if (Key == 99) {
            bin.copyTo(img);
        }

        // If user presses 'ESC', window will be closed
        if (Key == 27) {
            break;
        }
    }

    return 0;
}
