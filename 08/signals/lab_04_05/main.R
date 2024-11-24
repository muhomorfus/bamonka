library("stats")

# Gaussian impulse
gaussian_impulse <- function(x, s) {
  y <- exp(-(x / s)^2)
  return(y)
}

# Impulsive noise
impulsive_noise <- function(size, N, mult) {
  step <- floor(size / N)
  y <- rep(0, size)
  for (i in 1:floor(N / 2)) {
    y[round(size / 2) + i * step] <- mult * (0.5 + runif(1))
    y[round(size / 2) - i * step] <- mult * (0.5 + runif(1))
  }
  return(y)
}

# Butterworth filter
butterworth_filter <- function(D, size, type) {
  x <- seq(-size / 2, size / 2, length.out = size)
  if (type == 'low') {
    y <- 1 / (1 + (x / D)^4)
  } else if (type == 'high') {
    y <- 1 / (1 + (D / x)^4)
  } else {
    y <- x * sum(x)
  }
  y <- y / sum(y)
  return(y)
}

# Gaussian filter
gaussian_filter <- function(sigma, size, type) {
  x <- seq(-size / 2, size / 2, length.out = size)
  if (type == 'low') {
    y <- exp(-x^2 / (sigma^2))
  } else if (type == 'high') {
    y <- 1 - exp(-x^2 / (sigma^2))
  } else {
    y <- x * sum(x)
  }
  y <- y / sum(y)
  return(y)
}

# Initial state
sigma <- 0.5
mult <- 5
step <- 0.005
t <- seq(-mult, mult, by = step)

# Impulse
x0 <- gaussian_impulse(t, sigma)

# Gaussian noise generation
Na <- 0
NS <- 0.05
n1 <- rnorm(length(x0), mean = Na, sd = NS)
x1 <- x0 + n1

# Impulsive noise generation
count <- 7
M <- 0.4
n2 <- impulsive_noise(length(x0), count, M)
x2 <- x0 + n2

# Calculation of filtering arrays
B <- butterworth_filter(6, 20, 'high')
A <- butterworth_filter(6, 20, 'low')
GH <- gaussian_filter(4, 20, 'high')
GL <- gaussian_filter(4, 20, 'low')
BBH <- butterworth_filter(3, 20, 'high')
BBL <- butterworth_filter(3, 20, 'low')

# Plotting
par(mfrow=c(2,3))

# Initial signals
plot(t, x0, col = "red", type = "l", main = "Initial signals", ylab = "Amplitude", xlab = "Time")
lines(t, x1, col = "green")
lines(t, x2, col = "blue")
legend("topright", legend = c("No noise", "Gaussian noise", "Impulsive noise"), col = c("red", "green", "blue"), lty = 1)

# Gaussian filter (high freq)
plot(t, x0, col = "red", type = "l", main = "Gaussian filter (high freq)", ylab = "Amplitude", xlab = "Time")
lines(t, x1 - stats::filter(x=x1, GH, circular = TRUE), col = "green")
lines(t, x2 - stats::filter(x=x2, GH, circular = TRUE), col = "blue")
legend("topright", legend = c("No noise", "Gaussian noise", "Impulsive noise"), col = c("red", "green", "blue"), lty = 1)

# Gaussian filter (low freq)
plot(t, x0, col = "red", type = "l", main = "Gaussian filter (low freq)", ylab = "Amplitude", xlab = "Time")
lines(t, stats::filter(x=x1, filter=GL, circular = TRUE), col = "green")
lines(t, stats::filter(x2, GL, circular = TRUE), col = "blue")
legend("topright", legend = c("No noise", "Gaussian noise", "Impulsive noise"), col = c("red", "green", "blue"), lty = 1)

# Butterworth filter (high freq)
plot(t, x0, col = "red", type = "l", main = "Butterworth filter (high freq)", ylab = "Amplitude", xlab = "Time")
lines(t, x1-stats::filter(x1, BBH, circular = TRUE), col = "green")
lines(t, x2-stats::filter(x2, BBH, circular = TRUE), col = "blue")
legend("topright", legend = c("No noise", "Gaussian noise", "Impulsive noise"), col = c("red", "green", "blue"), lty = 1)

# Butterworth filter (low freq)
plot(t, x0, col = "red", type = "l", main = "Butterworth filter (low freq)", ylab = "Amplitude", xlab = "Time")
lines(t, stats::filter(x1, BBL, circular = TRUE), col = "green")
lines(t, stats::filter(x2, BBL, circular = TRUE), col = "blue")
legend("topright", legend = c("No noise", "Gaussian noise", "Impulsive noise"), col = c("red", "green", "blue"), lty = 1)

