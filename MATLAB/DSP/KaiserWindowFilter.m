%%%kaiser window
ws = 0.5*pi; %% elle girilcek
wp = 0.1*pi; %% elle girilcek
dw = ws - wp ; % deltaW degeri
%%% s degeri icin daha kucuk constraini alicaz
s = 0.01; %% elle girilcek
A = -20*log10(s);
if (A > 50)
    beta = 0.1102*(A-8.7);
end
if ((A >= 21) && (A <= 50)) 
    beta = 0.5842*(A-21)^(0.4) + 0.07866*(A-21);
end
if (A < 21)
    beta = 0;
end
M = ceil((A - 8)/(2.285*dw));
alpha = M / 2;
%%% windowun kendisi asagida
w = zeros(M, 1); %% window
for n = 0:M  
    x = beta*sqrt(1 - ((n-alpha)/alpha)^2);
    w(n+1) = besseli(0, x) / besseli(0, beta); 
end

%%%%%%
wc = ((ws+wp)/2); %% ws ve wp arasindaki degerin ortalamasi
M %%% M i hesapla
n = [-6:6]; %% M degerine gore belirleniyor yarisinin eksilisi ve artilisi
% sinc function
h = sin(wc*n) ./ (pi*n);
h(7) = 1; %%% HEM h ve w nun boyutlarini esit olmasi lazim ondan ekledik bunu
          %%% bu yuzden degisebilir bunlar elle degistirmek lazim

hlp = h .* w'; %% lowpass filter
plot(hlp)

n = [0:16000]; 

x = sin(0.05*pi*n) + sin(0.6*pi*n); %% her hangibir fonksiyon
for n = 1:13 % fonsiyonlarin boyutu
    y(n) = 0;
    for k = 1:16001 % x nin uzunlugu
        if (n-k+1) > 0
            y(n) = y(n) + x(k)*h(n-k+1);
        end
    end
end
%%y = conv(hlp, x); %%% fonksiyonun kaiser filtreden gecmis hali

subplot(2,1,1); plot( abs( fft(x) ) );
subplot(2,1,2); plot( abs( fft(y) ) );
