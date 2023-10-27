#!/usr/bin/env python3

import zplot

# Load the data
data = zplot.table(file='output.csv', separator=',')

# Create a canvas
c = zplot.canvas(title='TLB Size Measurement', canvas_type='pdf', dimensions=[1024, 768])

# Make a drawable region
d = zplot.drawable(c, xrange=[1, 65536], yrange=[0, 12], xscale='log2', yscale='linear', dimensions=[800, 600], coord=[100, 100])

m = data.getaxislabels('c0')
for pair in m:
    pair[1] = int(pair[0])

zplot.axis(drawable=d, title='TLB Size Measurement',
           xtitle='Number of Pages', xtitleshift=[0, -30],
           xlabelrotate=270, xlabelanchor='l', xlabelshift=[0, -2], xmanual=m,
           ytitle='Time per Access (ns)')

# Create a plotter and draw the data
p = zplot.plotter()
p.line(d, data, xfield='c0', yfield='c1', linewidth=2, linecolor='orange')
p.points(d, data, xfield='c0', yfield='c1', linewidth=2, fill=True, fillcolor='orange')

# Render and display the plot
c.render()
