%t = [1 2 3 4 5 6 7 8 9 10]
%Humidity = [10 20 30 40 50 60 70 80 90 10]
    s = load('time62.mat'); %Loads datatime

    T=readtable('tempfinalData.csv'); %csv file for Temperature export TempData.csv

    tempDouble=T{:,1};
        save('mat1.mat','tempDouble')

    H=readtable('Humidity62.csv'); %csv file for Humidity export

    humDouble=H{:,1};
        save('mat2.mat','humDouble')

%L=readtable('time.csv'); %csv file for time export; decided this was unnecessary
                         %this could theoretically be used instead of
                         %dataTime to record longer pieces of data.
    
%timeDouble=L{:,1};
%save('mat3.mat','timeDouble')
  %   ^^^^^^^^^----- your resulting .mat filename   
%time = timeDouble;
humidity = humDouble %humDouble;  
tempC = tempDouble; % convert to Celsius
%availableFields = ci.FieldDescriptions
  
%Calculating dewpoint  
b = 17.62; %these are constants
c = 243.5;

gamma = log(humidity/100) + b*tempC ./ (c+tempC); 
tdp = c*gamma ./ (b-gamma);
tdpf = (tdp*1.8) + 32;  % convert back to Fahrenheit; this can be switched out for Celcius if you wish

figure(2) %opens graphs for data analysis
        yyaxis right
    plot(t, humidity,'-r'); %charts humidity against time
        ylabel('% Relative Humidity')
        yyaxis left
    plot(t, tempC,'-g'); %charts temperature against time
       hold on
    plot(t, tdp,'-b');  %charts dewpoint against time
        ylabel('Degrees Celcius')
        legend({'Temperature','Dew point','Relative Humidity'},'Location', 'southwest')
       grid on
