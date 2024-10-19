#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

bool drawing = false;
Point lastPoint;
Scalar selectedColor(0, 0, 0);

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
        Mat* gcw = reinterpret_cast<Mat*>(userdata);
        uchar r = gcw->data[gcw->channels() * (gcw->cols * y + x) + 2];
        uchar g = gcw->data[gcw->channels() * (gcw->cols * y + x) + 1];
        uchar b = gcw->data[gcw->channels() * (gcw->cols * y + x) + 0];

        selectedColor = Scalar(b, g, r);
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
