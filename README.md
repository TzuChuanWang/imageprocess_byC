# imageprocess_by using C 

## 專案概述
使用c語言，並且不呼叫opencv和第三方api，實做基礎影像處理演算法

## 各檔案包含內容說明
* no1.cpp: 
  * 讀取RAW圖檔
  * 輸出RAW圖檔
  * 選轉照片
  * 隨機切分圖像區塊並打亂
  * 讀取四張正方型圖像，再還原成一張正方型圖像
  * 提升圖片亮度50 pixel
* no2.cpp:
  * 放大圖像局部區塊
  * 縮小圖像局部區塊
  * 使用nearest neighbor, bilinear , bicubic實現放大圖像
  * 比較先放大再縮小，和先縮小再放大差別
  * 計算MSE和PSNR
  * 讀取8位元圖像輸出1位元圖像
  * 最短路徑小遊戲，藉由不同規則D4, D8, Dm，從左上走到右下
* no3.cpp:
  * Bit plane: 將圖像藏至另外一張圖像後方位元  
  * 還原Bit plane
  * Grey level transformation : log transformation, power-law transformation
  * histogram equlization
* no4.cpp:
  * histogram matching
  * local histogram equlization 
  * image averaging : 藉由影像平均將道路上車子移除
  * edge detection : 使用矩陣實做卷積，並比較zero padding和 replicated padding方法
  * high-boost filter
  * gaussian filter
  * Difference of Gaussian(DoG)
* no5.cpp:
  * Homomorphic filter(rL, rH)
  * DFT
  * IDFT 
* no7.cpp:
  * periodic noise 
  * deblur_wiener
