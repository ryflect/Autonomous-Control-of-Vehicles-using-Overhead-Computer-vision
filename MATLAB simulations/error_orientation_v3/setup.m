
% ----------------------------------------------
% -------  swarm and control parameters  -------
% ----------------------------------------------
vehicle_length = 5; % (m)
%vehicle_length = 0.1; % (m)
vehicle_width  = 2; % (m)


X_ic = 0*[4*(rand-0.5)+3]; % (m) random ICs on X position, note: rand() is on [0 1]
Y_ic = 0*[5*(rand-0.5)+0]; % (m) random ICs on Y position
%yaw_ic = 2*pi*(rand(nAgents,1)-0.5); % (rad) random ICs for yaw, or heading
yaw_ic = 1*(-pi/2); % (rad) random ICs for yaw, or heading


% ---------------------------------------------
% -----  solver and animation parameters  -----
% ---------------------------------------------
h_fixed = 0.05; % (s) fixed solver simulation stepsize

plotAxisLimitMode = 0; % 0->auto, 1->fixed, use Axes Limits in 'plotAxisLimits'
%plotAxisLimits = [-3 5 -3 6]*20; % [xmin xmax ymin ymax]
plotAxisLimits = [-3 5 -3 6]*3; % [xmin xmax ymin ymax]

%anim_fps=20; % (animation frames / second)
anim_fps=10; % (animation frames / second)
%anim_fps=2; % (animation frames / second)
enable_CG_trace=1;       % (0/1) plot animation trace from vehicle CG, or geometric center
enable_rearAxle_trace=1; % (0/1) enable animation trace from rear axle

save_anim_frames=0; % (0/1) save animation frames? this slows the simulation considerably
                    %       when writing a .jpeg image to file at each animation interval. 
                    %       see writeVideo() at this link for converting into .avi movies:
                    %       http://www.mathworks.com/help/matlab/examples/convert-between-image-sequences-and-video.html)

% animation update rate assuming Simulink clock advances very nearly the wall clock
C = round(1/(anim_fps*h_fixed)); % see notes below
animation_update_interval=C*h_fixed; % (s) This parameter controls three things:
                                              %     (1) The animation update to the screen is updated at this
                                              %         rate which needs to be an integer multiple of fixed stepsize integration. 
                                              %     (2) A frame is saved on this interval during movie makin (i.e. when movie_parm.save_frames=1)
                                              %     (3) how close to (or how much faster than) real-time the simulation runs.
                                              %         See timing_notes.txt for more information on run speeds.


%radius = abs( vehicle_length / (-0.5) ); % (m) turn radius = L / R
%circumference = 2*pi*radius; % (m) distance traveled around the circle, once
%note: at v_x = 1(m/s), time for 1 revolution is circumference, or t=dist/vel 
% Assigning the path array
% P = [[20, 0]; [20, 10]; [20, 20]; [30, 30]; [40, 40]; [40, 50]; [40, 60]; [50, 70]; [60, 80]; [70, 80]; [80, 80]; [90, 80]; [100, 80]; [110, 90];...
%     [120, 100]; [120, 110]; [120, 120]; [110, 130]; [100, 140]; [100, 150]; [100, 160]; [100, 170]; [100, 180]; [90, 190]; [80, 200];...
%     [70, 190]; [60, 180]; [60, 170]; [60, 160]; [60, 150]; [50, 140]; [40, 130]; [30, 130]; [20, 130]; [10, 140]; [0, 150]; [0, 160];...
%     [0, 170]; [0, 180]; [0, 190]; [0, 200]; [0, 210]];
% p = 1;  % Index that goes through the path array
% current_position = P(p,:);
% closest_path_coordinate = P(p,:);
% Bring up the simulink model
% vehicle_model2a

vehicle_model_v3