function [res xdim ydim] = HalfNumericIntegration(func,xmin,xmax,ymin,ymax,stepsize)
%UNTITLED Summary of this function goes here
%  Calculate riemannsom of 2d function from xmin to xmax and ymin to ymax
%  with set stepsize.
    
    x = xmin:stepsize:xmax;
    y = ymin:stepsize:ymax;
    [dontcare xdim] = size(x);
    [dontcare ydim] = size(y);

    res = zeros(ydim,xdim);
    
    parfor i = 1:xdim
       for j = 1:ydim
           res(j,i) = func(x(i),y(j));
       end
    end   
end

