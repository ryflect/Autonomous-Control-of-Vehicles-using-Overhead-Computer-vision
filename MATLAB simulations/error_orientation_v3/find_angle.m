% Finding the angle between two points
function angle = find_angle(point1, point2)
%point1 is head of vector and point2 is tail
    angle = atan2((point1(1)-point2(1)),(point1(2)-point2(2)));
    if angle < 0
       angle = 2*pi + angle; 
    end
end
