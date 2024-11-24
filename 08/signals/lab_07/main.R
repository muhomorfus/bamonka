lab_07 <- function() {
  A <- 1.0
  s1 <- 1.0
  s2 <- 2.0
  mult <- 6
  step <- 0.009
  NS <- 0.05
  t <- seq(-mult, mult, by = step)
  
  u1 <- A * exp(-(t/s1)^2) # заданный исходный сигнал
  u2 <- A * exp(-(t/s2)^2) # искажённый сигнал
  
  v1 <- fft(u1)
  v2 <- fft(u2)
  
  n1 <- runif(length(t), -NS, NS)
  n2 <- runif(length(t), -NS, NS)
  
  delta <- sd(n1)
  epsilon <- sd(n2)
  

  library(stats)
    par(mfrow = c(1,1))
  plot(t, u1 + n1, type = "l", col = "blue", ylim = c(min(u1+n1, u2+n2), max(u1+n1, u2+n2)), 
       xlab = "", ylab = "", main = "")
  lines(t, u2 + n2, col = "green")
  lines(t, abs(ifft(fft(u2 + n2) * tikhonfilt(v1, v2, step, 2*mult, delta, epsilon))), col = "red")
  legend("topright", c('Заданный исходный сигнал','Поврежденный сигнал','Фильтр искажающий сигнал'), 
         lty = 1, col = c("blue", "green", "red"))
}

tikhonfilt <- function(u1, u2, step, T, d, e) { # возвращает линию, которая исказила сигнал
  m <- 0:(length(u1)-1)
  mult <- step / length(u1)
  squ <- 1 + (2 * pi * m / T)^2
  alpha <- uniroot(rhofunc, c(0, 1), u1 = u1, u2 = u2, step = step, T = T, d = d, e = e)$root
  h <- 0:(length(u1)-1)
  for (k in 1:length(h)) {
    h[k] <- mult * sum(exp(2 * pi * 1i * k * m / length(u1)) * u1 * Conj(u2) / (Mod(u2)^2 * step^2 + alpha * squ))
  }
  return(h)
}

rhofunc <- function(x, u1, u2, step, T, d, e) {
  m <- 0:(length(u1)-1)
  mult <- step / length(u1)
  squ <- 1 + (2 * pi * m / T)^2
  beta <- mult * sum(x^2 * squ * Mod(u1)^2 / (Mod(u2)^2 * step^2 + x * squ)^2)
  gamma <- mult * sum(Mod(u2)^2 * step^2 * Mod(u1)^2 * squ / (Mod(u2)^2 * step^2 + x * (1 + 2 * pi * m / T)^2)^2)
  return(beta - (d + e * sqrt(gamma))^2)
}

lab_07()