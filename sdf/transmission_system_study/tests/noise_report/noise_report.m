data = load('medidas_noise.txt');
x = data(:,1);
y = data(:,2);

semilogx(x,y,'o-')
grid
ylim([0,1])
xlabel('<n> - expected number of photons');
ylabel('var(n)');