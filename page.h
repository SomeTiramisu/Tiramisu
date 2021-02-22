#ifndef PAGE_H
#define PAGE_H

#include <opencv2/core.hpp>
#include <string>

class Page
{
public:
    Page();
private:
    cv::Mat img;
    int width;
    int height;
    int index;
    std::string book_filename;
};

#endif // PAGE_H
