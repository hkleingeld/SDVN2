function [result] = NumericIntegration_(func,xmin,xmax,ymin,ymax,stepsize)
%UNTITLED Summary of this function goes here
%  Calculate riemannsom of 2d function from xmin to xmax and ymin to ymax
%  with set stepsize.
    result = 0;
    
    x = xmin:stepsize:xmax;
    y = ymin:stepsize:ymax;
    [dontcare xdim] = size(x);
    [dontcare ydim] = size(y);
    
    xdim = xdim-1;
    ydim = ydim-1;
    res = zeros(ydim,xdim);
    
    parfor i = 1:xdim
       for j = 1:ydim
           res(j,i) = (func(x(i),y(j))+func(x(i+1),y(j+1)))/2 * stepsize^2;
       end
    end
%    ma = max(max(res));
%    mi = min(min(res));
    result = sum(sum(res));
%    average = result / (ydim*xdim);
%    ratio = mi/ma;
    
    
   
end

