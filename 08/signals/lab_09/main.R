library(remotes) # install first if necessary
remotes::install_github("deepayan/rip/rip.opencv")
remotes::install_github("deepayan/rip/rip.recover")

fpath <- "/Users/polnaya_katuxa/Study/Sem_08/signals/lab_09/bimage2.bmp"
img <- load.image(fpath)

par(mfrow = c(2,1))
plot(img)

img2 <- redR::RED(img, sigma = 1, lambda = 4, functional = 'DB', niter=5)

plot(img2)
# plot(img)
