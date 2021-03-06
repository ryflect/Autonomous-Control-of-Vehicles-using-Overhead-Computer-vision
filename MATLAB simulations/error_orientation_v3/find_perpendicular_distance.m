% Finding the perpendicular distance between two points
function dist = find_perpendicular_distance(point1, point2)
    dist = (point1(1)-point2(1)).^2 + (point1(2)-point2(2)).^2;
end