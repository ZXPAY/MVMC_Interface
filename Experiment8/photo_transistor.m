clc, clear, close all

filenames = ['photo_transistor_1.mat';'photo_transistor_2.mat';'photo_transistor_3.mat';'photo_transistor_4.mat';'photo_transistor_5.mat'];
fileNumber = 5;
dimension = 5;

data = zeros([100, dimension, 5]);
for i=1:fileNumber
    temp = load(filenames(i, :));
    temp = struct2cell(temp);
    for n=1:dimension
        data(:, n, i) = temp{n}.f0;     
    end
end

mean_data = mean(data);
std_data = std(data);

mean_data = reshape(mean_data, [dimension 5])
std_data = reshape(std_data, [dimension, 5])

for i=1:5
    fig = figure();
    plot([0:10:(dimension-1)*10], mean_data(:, i), 'r*--');
    grid on;
    xlabel('Current (mA)');
    ylabel('value');
    title('���PLED�G�׹�����ADC��');
    saveas(fig, strcat('_photoTransistor_mean_', num2str(i), '.jpg'));
    fig = figure();
    plot([0:10:(dimension-1)*10], std_data(:, i), 'b*--');
    grid on;
    xlabel('Current (mA)');
    ylabel('�зǮt');
    title('���PLED�G�׹������зǮt');
    saveas(fig, strcat('photoTransistor_std_', num2str(i), '.jpg'));
end




