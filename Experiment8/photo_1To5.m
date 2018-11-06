clc, clear, close all

filenames = ['photo_1.mat';'photo_2.mat';'photo_3.mat';'photo_4.mat';'photo_5.mat'];
fileNumber = 5;
dimension = 7;

data = zeros([100, dimension, 5]);
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

for i=1:5
    fig = figure();
    plot([1:1:dimension], mean_data(:, i), 'r*--');
    grid on;
    xlabel('Current (mA)');
    ylabel('value');
    title('���PLED�G�׹�����ADC��');
    saveas(fig, strcat('_photo_mean_', num2str(i), '.jpg'));
    fig = figure();
    plot([1:1:dimension], std_data(:, i), 'b*--');
    grid on;
    xlabel('Current (mA)');
    ylabel('�зǮt');
    title('���PLED�G�׹������зǮt');
    saveas(fig, strcat('photo_std_', num2str(i), '.jpg'));
end




