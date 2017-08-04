function P = random_path_generate(path_length)
%P = random_path_generate(path_length)
%Actual path length is path_length*segment_length
segment_length = 20;
initpos = [100,100];
P = zeros(path_length*segment_length,2);
%P(1:5,:) = [100,100;100,101;100,102;100,103;100,104;];

P(1:segment_length,1) = initpos(1);
for i=1:segment_length
   P(i,2) = initpos(2) + (i-1);
end

rowchange = 0;
colchange = 0;
previous_orientation = 3;

for i=2:path_length
   random_orientation = randi(5); %5 possible directions to move
   if random_orientation == 1
       rowchange = -1;
       colchange = 0;
   elseif random_orientation == 2
       rowchange = -1;
       colchange = 1;
   elseif random_orientation == 3
       rowchange = 0;
       colchange = 1;
   elseif random_orientation == 4
       rowchange = 1;
       colchange = 1;
   elseif random_orientation == 5
       rowchange = 1;
       colchange = 0;
   end
   
   for j=(i-1)*segment_length+1:i*segment_length
       P(j,:) =  [P(j-1,1) + rowchange, P(j-1,2) + colchange];
       if (P(j,1) == 0) || ((previous_orientation == 5)&&(random_orientation == 1)) || ((previous_orientation == 1)&&(random_orientation == 5)) || ((previous_orientation == 4)&&(random_orientation == 1)) || ((previous_orientation == 2)&&(random_orientation == 5)) || ((previous_orientation == 1)&&(random_orientation == 4)) || ((previous_orientation == 5)&&(random_orientation == 2))
           rowchange = 0;
           colchange = 1;
           P(j,:) =  [P(j-1,1) + rowchange, P(j-1,2) + colchange];
       end
      
   end
   previous_orientation = random_orientation;
end


%visualization
rowmax = max(P(:,1));
colmax = max(P(:,2));

Grid = ones(rowmax*2,colmax*2);

for i=1:length(P)
   Grid(P(i,1),P(i,2)) = 0; 
end

imtool(Grid)

end