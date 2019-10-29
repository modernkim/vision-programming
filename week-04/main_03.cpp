#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int peak[] = {};
int Th_01 = 0;
float inclination_value = 0.0;
float inclination[255] = {};
float inclination_pt_x[255] = {};
float inclination_before_value[255] = {};

void calc_histo(Mat image, Mat &hist, int bins, int range_max = 256)
{
    int histSize[] = {bins};
    float range[] = {0, (float)range_max};
    int channels[] = {0};
    const float *ranges[] = {range};
    calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges);

    // cout << histSize << endl;
}

void draw_histo(Mat hist, Mat &hist_img, Size size = Size(256, 200))
{
    hist_img = Mat(size, CV_8U, Scalar(255));
    int count = 0;

    float bin = (float)hist_img.cols / hist.rows;
    normalize(hist, hist, 0, hist_img.rows, NORM_MINMAX);

    for (int i = 0; i < hist.rows; i++)
    {
        float start_x = i * bin;
        float end_x = (i + 1) * bin;

        Point2f pt_01(start_x, 0);
        Point2f pt_02(end_x, hist.at<float>(i));
        Point2f pt_03(start_x + 1, 0);
        Point2f pt_04(end_x, hist.at<float>(i + 1));

        if (pt_02.y >= 0)
        {
            inclination_before_value[i] = pt_02.y;
            // cout << "현재 pt_04.x 의 y value = " <<inclination_before_value[i] << endl
            //          << endl;
            // cout << "이전 pt_02.x-5의 y value = " <<inclination_before_value[i - 4] << endl;

            if (i % 15 == 0)
            {
                inclination[count] = ((pt_04.y - inclination_before_value[i - 15]) / (pt_03.x - pt_01.x - 15));
                inclination_pt_x[count] = pt_01.x;
            }
            rectangle(hist_img, pt_01, pt_02, Scalar(0), -1);
            count++;
        }
    }

    int floatCnt = sizeof(inclination) / sizeof(inclination[0]);

    for (int i = 0; i < floatCnt; i++)
    {
        if (inclination_value < inclination[i])
        {
            inclination_value = inclination_pt_x[i];
            Th_01 = inclination_pt_x[i];
            cout << inclination_pt_x[i] << endl;
        }
    }
    flip(hist_img, hist_img, 0);
}
int main()
{

    Mat image_01 = imread("./assets/cellimg2.jpg", 0);
    Mat image_02 = imread("./assets/cellimg2.jpg", 0);
    CV_Assert(!image_01.empty());

    Mat hist, hist_img_01;

    calc_histo(image_01, hist, 256);
    draw_histo(hist, hist_img_01);

    for (int i = 0; i <= image_01.rows; i++)
    {
        for (int j = 0; j <= image_01.cols; j++)
        {

            if (image_01.at<uchar>(i, j) >= Th_01)
            {
                image_02.at<uchar>(i, j) = 255;
            }
        }
    }

    cout << image_01.size() << endl;
    imshow("hist_img_01", hist_img_01);
    imshow("image_01", image_01);
    imshow("image_02", image_02);

    waitKey(0);
    return 0;
}
