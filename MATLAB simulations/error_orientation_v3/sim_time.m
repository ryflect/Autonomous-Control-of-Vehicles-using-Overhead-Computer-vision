sim_times = linspace(0, 351, 352);
steering_angles = [45, -45, 45, 45, -45, -45, -45, 45, -45, -90, -45, 45, 45, 45, 45];
steering_times = [20, 40, 60, 80, 120, 140, 160, 180, 220, 240, 260, 290, 310, 330, 350];
y = 0;
flag = 0;
for x = 1:length(steering_angles)
    while flag == 0 
        if steering_times(x) == sim_times
            steering_force = 0.5 * (steering_angles(x)/45);
            y = steering_force;
            t = sim_times;
            flag = 1;
            while sim_times ~= t+5
                continue
            end
        else
            y = 0;
        end
    end
    flag = 0;
end