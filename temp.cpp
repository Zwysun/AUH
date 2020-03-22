//Step#3.截取圖片部分，提高識別率get ROI and save as szDealPic
		if (pic1rect.x >= 0 && pic1rect.y >= 0 && pic1rect.width > 0 && pic1rect.height > 0)
		{
			IplImage *img1 = NULL;
 
			img1 = cvLoadImage(szTakePhotoName);	//load original image
			if (img1 == NULL)
			{
				LOG_FOR_RD("Load test image fail!please check picture!");
				MessageBox(NULL, "Load test image fail!please check picture!", NULL, NULL);
				return TEST_FAIL;
			}
			cvSetImageROI(img1, pic1rect);// intercept img1 size of pic1rect   圖像截取Set Image ROI
			if (iPicColorRangeL[0] > 0 || iPicColorRangeL[1] > 0 || iPicColorRangeL[2] > 0 || iPicColorRangeH[0] < 255 || iPicColorRangeH[1] < 255 || iPicColorRangeH[2] < 255)
			{
				IplImage *img2 = NULL, *img3 = NULL, *img4 = NULL;
				img2 = cvCreateImage(cvGetSize(img1), IPL_DEPTH_8U, 1);  //創建灰度图像，大小和img1一樣
				img3 = cvCreateImage(cvGetSize(img1), IPL_DEPTH_8U, 1);
				img4 = cvCreateImage(cvGetSize(img1), IPL_DEPTH_8U, 1);
				//检查图像中的像素灰度是否属于某一指定范围
				cvInRangeS(img1, cvScalar(iPicColorRangeL[2], iPicColorRangeL[1], iPicColorRangeL[0]), cvScalar(255, 255, 255), img2);
				cvInRangeS(img1, cvScalar(iPicColorRangeH[2], iPicColorRangeH[1], iPicColorRangeH[0]), cvScalar(255, 255, 255), img3);
				cvAnd(img2, img3, img4);//計算两个数组的按位与的结果
 
				cvSaveImage(szDealPic, img4);//保存文件
			}
			else
				cvSaveImage(szDealPic, img1);
		}
		else
			::CopyFile(szTakePhotoName, szDealPic, TRUE);
