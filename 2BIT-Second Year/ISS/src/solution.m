%Autor: Marek Marusic, xmarus05
%email: xmarus05@stud.fit.vutbr.cz

img = imread('xmarus05.bmp');

H = [-0.5 -0.5 -0.5; -0.5 5.0 -0.5; -0.5 -0.5 -0.5]
img1 = imfilter(img, H);
imwrite(img, 'step1.bmp');

%Ixt = imread('ref/step1.bmp');
%subplot(121); imshow(img1); subplot(122); imshow(Ixt);
%sum(sum(abs(double(img1) - double(Ixt))))

img2 = fliplr(img1);
imwrite(img2, 'step2.bmp');

%Ixt = imread('ref/step2.bmp');
%subplot(121); imshow(img2); subplot(122); imshow(Ixt);
%sum(sum(abs(double(img2) - double(Ixt))))

img3 = medfilt2(img2, [5 5]);
imwrite(img3, 'step3.bmp');

%Ixt = imread('ref/step3.bmp');
%subplot(121); imshow(img3); subplot(122); imshow(Ixt);
%sum(sum(abs(double(img3) - double(Ixt))))

H_rozmazani = [1 1 1 1 1; 1 3 3 3 1; 1 3 9 3 1; 1 3 3 3 1; 1 1 1 1 1]/49;
img4 = imfilter(img3, H_rozmazani);
imwrite(img4, 'step4.bmp');

%Ixt = imread('ref/step4.bmp');
%subplot(121); imshow(img4); subplot(122); imshow(Ixt);
%sum(sum(abs(double(img4) - double(Ixt))))

noise = 0;
I = double(img);
I2 = double(fliplr(img4));
for (x=1:512)
   for (y=1:512)
       noise = noise + double(abs(I(x,y)-I2(x,y)));
   end;
end;

noise = noise/512/512


imgMin = min(min(im2double(img4)));
imgMax = max(max(im2double(img4)));
img5 = imadjust(img4, [imgMin imgMax], [0 1]);
imwrite(img5, 'step5.bmp');

%Ixt = imread('ref/step5.bmp');
%subplot(121); imshow(img5); subplot(122); imshow(Ixt);
%sum(sum(abs(double(img5) - double(Ixt))))

mean_no_hist = mean2(double(img4))
std_no_hist = std2(double(img4))
mean_hist = mean2(double(img5))
std_hist = std2(double(img5))


N = 2;
a = 0;
b = 255;

img5Double = double(img5);
img6 = zeros(512,512);
for k=1:512
   for l=1:512
      img6(k,l) = round(((2^N)-1)*(img5Double(k,l)-a)/(b-a))*(b-a)/((2^N)-1) + a;
   end
end

img6=uint8(img6);
imwrite(img6, 'step6.bmp');

%Ixt = imread('ref/step6.bmp');
%subplot(121); imshow(img6); subplot(122); imshow(Ixt);
%sum(sum(abs(double(img6) - double(Ixt))))
