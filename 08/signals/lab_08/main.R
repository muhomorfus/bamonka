lab08 <- function(){
  F <- 3
  dt <- 0.05
  x <- seq(-F, F, dt)
  yx <- mygaussignal(x)
  uxbase <- mygaussignal(x)
  ux <- mygaussignal(x)
  N <- length(yx)
  
  a <- 0.25
  epsv <- 0.05
  
  px <- a * runif(7)
  
  pos <- c(25, 35, 40, 54, 67, 75, 95)
  pxx <- length(pos)
  
  for (i in 1:pxx){
    ux[pos[i]] <- ux[pos[i]] + px[i]
    uxbase[pos[i]] <- uxbase[pos[i]] + px[i]
  }
  
  for (i in 1:N){
    smth <- mean(ux, i)
    if (abs(ux[i] - smth) > epsv){
      ux[i] <- smth
    }
  }
  
  par(mfrow = c(2,1))
  plot(x, yx, type='l', col='blue', xlab='x', ylab='y', main='MEAN-функция фильтрации')
  lines(x, uxbase, col='green')
  lines(x, ux, col='red')
  legend("topright", c('Исходный', 'Искаженный', 'Сглаженный'), fill=c('blue', 'green', 'red'))
  
  uxbase <- mygaussignal(x)
  ux <- mygaussignal(x)
  
  for (i in 1:pxx){
    ux[pos[i]] <- ux[pos[i]] + px[i]
    uxbase[pos[i]] <- uxbase[pos[i]] + px[i]
  }
  
  for (i in 1:N){
    smth <- med(uxbase, i)
    if (abs(ux[i] - smth) > epsv){
      ux[i] <- smth
    }
  }
  
  plot(x, yx, type='l', col='blue', xlab='x', ylab='y', main='MED-функция фильтрации')
  lines(x, uxbase, col='green')
  lines(x, ux, col='red')
  legend("topright", c('Исходный', 'Искаженный', 'Сглаженный'), fill=c('blue', 'green', 'red'))
}

mean <- function(ux, i){
  r <- 0
  imin <- i - 2
  imax <- i + 2
  for (j in imin:imax){
    if (j > 0 && j < (length(ux) + 1)){
      r <- r + ux[j]
    }
  }
  r <- r / 5
  return(r)
}

med <- function(ux, i){
  imin <- i - 1
  imax <- i + 1
  if (imin < 1){
    ir <- ux[imax]
  } else {
    if (imax > length(ux)){
      ir <- ux[imin]
    } else {
      if (ux[imax] > ux[imin]){
        ir <- ux[imin]
      } else {
        ir <- ux[imax]
      }
    }
  }
  return(ir)
}

mygaussignal <- function(x){
  a <- 1
  sigma <- 1
  y <- a * exp(-x^2 / sigma^2)
  return(y)
}

lab08()