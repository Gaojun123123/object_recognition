# Single Color RGB565 Blob Tracking Example
#
# This example shows off single color RGB565 tracking using the OpenMV Cam.

import sensor, image, time, math
from pyb import UART
threshold_index = 0 # 0 for red, 1 for green, 2 for blue

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green/blue things. You may wish to tune them...
thresholds = [ # generic_red_thresholds


                (24, 39, -51, 32, 18, 52),
                (24, 51, -51, -26, 23, 51),
                (11, 62, 2, 45, -10, 50),
                (8, 28, 7, 54, -15, 37),
                (31, 49, -54, -8, 23, 69),
                #绿色
                (23, 87, -7, 52, 12, 43),
                (23, 86, 1, 52, 10, 52),
                (26, 47, -55, -21, 26, 58),
                (11, 23, 7, 48, -6, 48),
                (12, 20, -18, 51, -18, 41),
                #红色
                (57, 72, -35, -17, -20, 1),
                (54, 73, -67, -11, -27, -3),
                (31, 47, -38, -12, -24, 0),
                (28, 51, -31, -14, -15, 2),
                (30, 48, -30, -11, -19, 0)
                #蓝色


             ] # generic_blue_thresholds

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
#sensor.set_framesize(sensor.QVGA)
sensor.set_framesize(sensor.QVGA) # Special 128x160 framesize for LCD Shield.


sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()
uart = UART(3, 9600) #初始化串口3
output_str ='1' #定义输出
shape=0 #形状的标识符 圆 ：1 正方形 ：2  三角形 ：3
Size=0  #尺寸的标识符

# Only blobs that with more pixels than "pixel_threshold" and more area than "area_threshold" are
# returned by "find_blobs" below. Change "pixels_threshold" and "area_threshold" if you change the
# camera resolution. "merge=True" merges all overlapping blobs in the image.
arr=[] #定义数组
while(True):
    print(clock.fps())
    clock.tick()
    img = sensor.snapshot()
    for blob in img.find_blobs(thresholds, pixels_threshold=400, area_threshold=400, merge=True):
        # These values depend on the blob not being circular - otherwise they will be shaky.
        if blob.elongation() > 0.5:
            img.draw_edges(blob.min_corners(), color=(255,0,0))
            img.draw_line(blob.major_axis_line(), color=(0,255,0))
            img.draw_line(blob.minor_axis_line(), color=(0,0,255))
        # These values are stable all the time.
        points=blob.min_corners()
        x1=math.pow(points[1][0]-points[0][0],2)#(x1-x2)^2
        x2=math.pow((points[1][1]-points[0][1]),2)
        distance=math.sqrt(x1+x2)#计算边长
        img.draw_edges(blob.min_corners(), color=(255,0,0))
        areas=distance*distance #外接为正方形
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())
        result=blob.pixels()/areas #pixels
         #数据进行滤波处理


        #根据面积与最小外接矩形的比值选择是那个图形
        if result<=1.1 and result>0.9:  #正方形
           shape=1
        elif result<=0.8 and result>0.6: #圆
           shape=2
        elif result<=0.6 and result>0.4:#三角形
           shape=3
        else:
           shape=0
        #print(shape)
        Size=distance #边长或直径
        print(Size)
        output_str="[S%dX%dY%dR=%dW]" % (shape,blob.cx(),blob.cy(),Size) #要发送的数据
        #print(output_str)
        uart.write(output_str+'\r\n')  #输出到串口3

       #  print('r')
       #print(blob.rotation())
       # Note - the blob rotation is unique to 0-180 only.
       # img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)

