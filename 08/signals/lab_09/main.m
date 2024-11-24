img=double(imread('bimage2.bmp')) / 255;

figure;
imshow(img); 
title('Source image');

p1=fspecial('motion', 55, 205);
% PSF=fspecial('motion', 54, 65);
[restored p2]=deconvblind(img, p1);
figure;
imshow(restored);
title('Recovered image');