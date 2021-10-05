
A = [2 4 2 0 0]; % bolumun altinda ki katsayilarin yuksekten dusuge yazilimi
B = [1 -3 2]; % bolumun ustunde ki katsayilarin yuksekten dusuge yazilimi
%%% ornek olarak (A/B) degeri gibi 
[r,p,k] = residue(A, B);

r; %bolumun ustunde ki degerler ilk deger r1 direk ustte sonra r2 sonra r3 vsvsvs...
p; %bolumun altindaki degerler  ilk alttaki deger s-p1 ... s-p2 ikinci deger gibi
k; %tamsayi arta kalan ..... + 5 gibi veya sonuc [2 0 -2] ise 2s^2 + 0s + (-2) gibi

%%% s degisken deger (x gibi)
%%% genel olarak sonuc = (r1/(s-p1)) + (r2/(s-p2)) + .... + k seklinde

