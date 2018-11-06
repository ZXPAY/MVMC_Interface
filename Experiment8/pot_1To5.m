clc, clear, close all

filenames = ['1.mat';'2.mat';'3.mat';'4.mat';'5.mat'];
fileNumber = 5;
dimension = 7;

data = zeros([100, dimension, fileNumber]);
for i=1:fileNumber
    temp = load(filenames(i, :));
    temp = struct2cell(temp);
    for n=1:7
        data(:, n, i) = temp{n}.f0;     
    end
end

mean_data = mean(data);
std_data = std(data);

mean_data = reshape(mean_data, [dimension 5])
std_data = reshape(std_data, [dimension, 5])

for i=1:fileNumber
    fig = figure();
    plot([1:45:dimension*45], mean_data(:, i), 'r*--');
    grid on;
    xlabel('angle (度)');
    ylabel('value');
    title('不同旋轉角度對應的ADC值');
    saveas(fig, strcat('_pot_mean_', num2str(i), '.jpg'));
    fig = figure();
    plot([1:45:dimension*45], std_data(:, i), 'b*--');
    grid on;
    xlabel('angle (度)');
    ylabel('標準差');
    title('不同旋轉角度對應的標準差');
    saveas(fig, strcat('pot_std_', num2str(i), '.jpg'));
end




