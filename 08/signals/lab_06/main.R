gauspls <- function(x, A, s) {
  y <- A * exp(-(x / s)^2)
  return(y)
}

impnoise <- function(size, N, mult) {
  step <- floor(size / N)
  y <- rep(0, size)
  for (i in 1:floor(N / 2)) {
    y[round(size / 2) + i * step] <- mult * (0.5 + runif(1))
    y[round(size / 2) - i * step] <- mult * (0.5 + runif(1))
  }
  return(y)
}

wiener <- function(x,n) {
  y <- 1 - (n/x)^2
  return(y)
}

rejection <- function(x,n) {
  y <- rep(0, length(x))
  for (i in 1:length(x)) {
    if (abs(x[i]) - abs(n[i]) > 1) {
      y[i] <- 1
    } else {
      y[i] <- 0
    }
  }
  return(y)
}

A <- 1.0
sigma <- 0.5

mult <- 5
step <- 0.005
t <- seq(-mult, mult, by = step)

x0 <- gauspls(t, A, sigma)

Na <- 0
NS <- 0.05
n1 <- rnorm(length(x0), mean = Na, sd = NS)
x1 <- x0 + n1

count <- 7
M <- 0.4
n2 <- impnoise(length(x0), count, M)
x2 <- x0 + n2

y1 <- wiener(fft(x1), fft(n1))
y2 <- wiener(fft(x2), fft(n2))

z1 <- rejection(fft(x1), fft(n1))
z2 <- rejection(fft(x2), fft(n2))


library(stats)

# PLOTTING
par(mfrow = c(2,1))
plot(t, x1, type = 'l', col = "blue", main = "Фильтрация гауссовой помехи фильтром Винера")
lines(t, Re(ifft(fft(x1) * y1)), col = "red")
lines(t, Re(ifft(fft(x1) * z1)), col = "green")

plot(t, x2, type = 'l', col = "blue", main = "Фильтрация импульсной помехи фильтром Винера")
lines(t, Re(ifft(fft(x2) * y2)), col = "red")
lines(t, Re(ifft(fft(x2) * z2)), col = "green")


