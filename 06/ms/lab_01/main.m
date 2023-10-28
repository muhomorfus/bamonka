X = [1.52, 1.26, 2.17, 1.75, -0.19, 2.24, 2.76, 1.52, 1.89, 3.10, 2.61, 1.18, 1.83, 1.85, 3.39, 2.31, 2.99, 1.61, 2.57, 1.81, 1.73, 1.89, -0.00, 2.27, 1.61, 2.57, 2.54, 1.67, 1.49, 0.12, -0.04, 1.36, 2.04, 2.04, -0.05, 0.67, 1.32, 0.78, 0.89, 2.73, 1.51, 1.48, 1.67, 2.18, 1.70, 4.20, 1.81, 2.66, 1.72, 0.77, 3.16, 1.86, 3.66, 4.30, 0.98, 3.00, 0.99, 1.72, 2.71, 2.47, 2.56, 1.99, 0.23, 0.66, 2.47, 2.71, 2.28, 2.59, 3.30, 2.08, 0.90, 0.49, 2.38, 0.71, 0.10, 1.50, 0.21, 0.44, 3.94, 1.50, 1.70, -0.73, 1.76, 2.71, 1.95, -0.71, 1.32, 3.95, 2.64, -0.04, 3.24, 1.67, 2.31, 0.18, 0.79, 3.26, 3.44, 2.64, 0.89, 2.47, 4.02, 2.12, 0.61, 2.59, 1.44, 1.82, 2.94, 3.03, 1.97, 2.30, 0.80, 0.52, 1.21, 2.13, 2.82, 1.56, 2.84, 3.54, 0.86, 0.42];
%X = [X X X X X];
X = [X zeros(1,100)-1 zeros(1,200)+5];
X = X.*(sin(X));
M_max = max(X)
M_min = min(X)

R = M_max - M_min

n = length(X);

mu = sum(X) / n
s2 = sum((X - mu) .^ 2) / (n - 1)

m = round(log2(n)) + 2;

[counts, edges] = histcounts(X, m, 'BinLimits', [M_min, M_max])

delta = R / m;
step = delta / 10;
Xs = M_min:step:M_max;
Ys = normpdf(Xs, mu, sqrt(s2));

hold on;
h = histogram();
h.BinEdges = edges;
h.BinCounts = counts / (n * delta);

plot(Xs, Ys, "red");

figure;
hold on;

[Ye, Xe] = ecdf(X);

Xe = [-3; Xe; 10];
Ye = [0; Ye; 1];

axis([-3 10 0 1]);

stairs(Xe, Ye, "blue");

Xs1 = -3:step:10;

% Ys1 = normcdf(Xs1, mu, s2);
% plot(Xs1, Ys1, "red");

Ys1 = zeros(1, length(Xs1));
func = @(x) 1 / (sqrt(s2) * sqrt(2 * pi)) * exp(-(x - mu).* (x - mu) / (2 * s2));
for i = 1:length(Xs1)
    Ys1(i) = integral(func, -Inf, Xs1(i));
end

plot(Xs1, Ys1, "red");