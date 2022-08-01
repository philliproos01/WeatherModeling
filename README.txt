a) Make sure all the necessary files are in a folder together. The main.m contains the main source code.

b)  
Functionality of some of the functions:

    s = load('time62.mat'); %Loads datatime

    T=readtable('tempfinalData.csv'); %csv file for Temperature export TempData.csv

    tempDouble=T{:,1};
        save('mat1.mat','tempDouble')

    H=readtable('Humidity62.csv'); %csv file for Humidity export

    humDouble=H{:,1};
        save('mat2.mat','humDouble')

********************************************************************************************************************************************
The functions above read the csv files which includes the exported temperature and humidity data from the device and transfer them into
readable doubles with a size of [62x1].
********************************************************************************************************************************************


%Calculating dewpoint  
b = 17.62; %these are constants
c = 243.5;

gamma = log(humidity/100) + b*tempC ./ (c+tempC); 
tdp = c*gamma ./ (b-gamma);
tdpf = (tdp*1.8) + 32;  % convert back to Fahrenheit; this can be switched out for Celcius if you wish


********************************************************************************************************************************************
The above equations calculate the dew point from each temperature and humidity dataset. An optional Fahrenheit conversion is also available 
the user wishes to modify the code to outbound Fahrenheit data.
********************************************************************************************************************************************

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

********************************************************************************************************************************************
The calculated and read data is than plotted on a single graph, giving an accurate depiction of the weather data. Without knowledge of 
cloud cover, atmospheric pressure, or wind speed, this will also give the user the best ability for what the near future weather will look like.
********************************************************************************************************************************************


c) Unfortunately you can not run the physical device as I have that. 
I have provided the necessary exported data in the form of of tempfinalData.csv and Humidity62.csv
Open up the "main.m" file in matlab. Next, open the "time62.mat". Make sure the datatime table "t" is active- it should show up in
the workspace. Finally, run the "main.m" script!
 