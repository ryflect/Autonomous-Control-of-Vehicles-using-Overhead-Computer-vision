% finding the closest path co-ordinate
function [closest,closest_index] = find_closest_path_coordinate(P, currentpos)
    dist1 = 1e6;
    closest = 0;
    closest_index = 1;
    for i = 1:length(P)
        dist = find_perpendicular_distance(P(i,:), currentpos);
        if dist < dist1
            dist1 = dist;
            closest = P(i,:);
            closest_index = i;
        end
    end
end