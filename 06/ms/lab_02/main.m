%x = [-8.47, -7.45, -7.12, -8.30, -8.15, -6.01, -5.20, -7.38, -6.76, -9.18, -6.00, -8.08, -7.96, -8.34, -6.82, -8.46, -8.07, -7.04, -7.24, -8.16, -8.20, -8.27, -7.79, -7.37, -7.02, -7.13, -6.99, -7.65, -8.18, -6.71, -8.41, -6.71, -7.04, -9.15, -7.74, -10.11, -8.20, -7.07, -7.63, -8.99, -6.62, -6.23, -7.13, -6.41, -7.06, -7.72, -8.44, -8.85, -8.02, -6.98, -6.08, -7.20, -7.48, -7.82, -9.19, -8.31, -7.95, -7.97, -6.66, -6.59, -9.10, -7.87, -9.02, -8.77, -7.62, -9.44, -8.05, -7.60, -7.33, -6.94, -8.51, -7.39, -6.44, -8.88, -8.21, -7.66, -6.91, -8.39, -7.37, -7.26, -6.04, -7.58, -7.28, -7.02, -7.10, -7.33, -8.63, -8.21, -7.12, -8.11, -9.03, -8.11, -8.79, -9.22, -7.32, -5.97, -7.26, -6.39, -7.64, -8.38, -7.67, -7.70, -7.70, -8.95, -6.25, -8.09, -7.85, -8.10, -7.73, -6.78, -7.78, -8.20, -8.88, -8.51, -7.45, -7.14, -6.63, -7.38, -7.72, -6.25];
[min(x) max(x)];
x = zeros(1,120)-2;
x = [x zeros(1,120)];
gamma = 0.9;

n = length(x);

mu = sum(x) / n
s2 = sum((x - mu) .^ 2) / (n - 1)

mu_min = mu - sqrt(s2) * tinv((1 + gamma) / 2, n - 1) / sqrt(n)
mu_max = mu + sqrt(s2) * tinv((1 + gamma) / 2, n - 1) / sqrt(n)

s2_min = s2 * (n - 1) / chi2inv((1 + gamma) / 2, n - 1)
s2_max = s2 * (n - 1) / chi2inv((1 - gamma) / 2, n - 1)

ns = zeros(1, n);

mus = zeros(1, n);
mu_mins = zeros(1, n);
mu_maxes = zeros(1, n);

s2s = zeros(1, n);
s2_mins = zeros(1, n);
s2_maxes = zeros(1, n);

mu_const = zeros(1, n);
s2_const = zeros(1, n);

for i = 1:n
     ns(i) = i;
     locx = x(1:i);
 
     mu_const(i) = mu;
     s2_const(i) = s2;
 
     mus(i) = sum(locx) / i;
     s2s(i) = sum((locx - mus(i)) .^ 2) / (i - 1);
 
     mu_mins(i) = mus(i) - sqrt(s2s(i)) * tinv((1 + gamma) / 2, i - 1) / sqrt(i);
     mu_maxes(i) = mus(i) + sqrt(s2s(i)) * tinv((1 + gamma) / 2, i - 1) / sqrt(i);
 
     s2_mins(i) = (i - 1) * s2s(i) / chi2inv((1 + gamma) / 2, i - 1);
     s2_maxes(i) = (i - 1) * s2s(i) / chi2inv((1 - gamma) / 2, i - 1);
 end

start = 7;

plot(start:n, mu_const(start:n), start:n, mus(start:n), start:n, mu_mins(start:n), start:n, mu_maxes(start:n), 'LineWidth', 1.5);
title('$\hat \mu$','interpreter','latex', 'FontSize', 14);
xlabel('n');
ylabel('y');
legend('$\hat \mu(\vec x_N)$', '$\hat \mu(\vec x_n)$', '$\underline{\mu}(\vec x_n)$', '$\overline{\mu}(\vec x_n)$', 'Interpreter', 'latex', 'FontSize', 14);

figure;

start = 13;

plot(start:n, s2_const(start:n), start:n, s2s(start:n), start:n, s2_mins(start:n), start:n, s2_maxes(start:n), 'LineWidth', 1.5);
title('$\hat S^2$','interpreter','latex', 'FontSize', 14);
xlabel('n');
ylabel('z');
legend('$\hat S^2(\vec x_N)$', '$\hat S^2(\vec x_n)$', '$\underline{\sigma}^2(\vec x_n)$', '$\overline{\sigma}^2(\vec x_n)$', 'Interpreter', 'latex', 'FontSize', 14);




