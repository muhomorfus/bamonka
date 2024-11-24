library("signal")

# Прямоугольный импульс
rect <- function(e) as.integer(abs(e) < 1)
uRect <- function(x, l) rect(x / l)

# Сигнал Гаусса
a <- 1
uGauss <- function(x, sigma) a * exp(-(x / sigma)^2)

# Исправление функции.
fix <- function(u) {
  for (i in 1:length(u)) {
    u[i] <- (-1)^(i-1) * u[i]
  }
  u
}

convolution <- function(x, f1, f2, step) {
  # 1. Дополнение нулями.
  f1Extended <- c(f1, numeric(length(f1)))
  # 2. Дополнение нулями второй функции.
  f2Extended <- c(f2, numeric(length(f2)))
  # 3. Вычислить БПФ от каждой функции - получим 2 спектра
  v1 <- fft(f1Extended)
  v2 <- fft(f2Extended)
  # 4. Вычислить произведение V1 * V2
  w <- v1 * v2
  # 5. Вычислить обратное БПФ от функции W
  res <- abs(ifft(w)) * step
  #res <- convolve(f1Extended, f2Extended, type='open') * step
  start <- (length(f1Extended) - length(x)) / 2 + 1
  res[start:(start + length(x) - 1)]
}

l <- 3.0
sigma <- 1.0

f <- 5
step <- 0.05

t <- seq(-f, f, step)

rectPoints <- uRect(t, l)
rectHalfPoints <- uRect(t, l/2)

gaussPoints <- uGauss(t, sigma)
gaussHalfPoints <- uGauss(t, sigma/2)

plotConvolution <- function(x, f1, f2, title) {
  cnv <- convolution(x, f1, f2, step)
  
  plot(x, f1, type='l', col='blue', xlab='t', ylab='Amplitude', main=title, ylim=c(0, max(c(f1, f2, cnv))))
  lines(x, f2, col='purple')
  lines(x, cnv, col='red')
  legend('topright', legend=c('Signal 1', 'Signal 2', 'Convolution'), col=c('blue', 'purple', 'red'), lty=1, cex=0.5)
}

par(mfrow=c(1, 3))
plotConvolution(t, rectPoints, gaussPoints, 'rect(x) + Гаусс')
plotConvolution(t, rectPoints, rectHalfPoints, 'rect(x) + rect(x)')
plotConvolution(t, gaussPoints, gaussHalfPoints, 'Гаусс + Гаусс')

