# Site Selector

This program was made during my internship at SkyWater Technologies, a semiconductor manufacturer based in Minnesota. My goal was to create an algorithm that picked testing sites, or dies, on a silicon wafer such that sites were evenly distributed over the area. I then had to output a file in the `.waf` file format with the selected coordinates on the wafer. I use two methods to pick sites:

## Random Site Selection
This method has $5$ use cases. 

- $1$ site: Pick a random site anywhere on the wafer 
- $2$ sites: Pick a random site in two sections -- a smaller circle in the middle of the wafer with a radius of $r/4$, where $r$ is the radius of the wafer 
- $3$ sites: Pick a random site in three sections -- the smaller circle mentioned above, the top half of the wafer, and the bottom half of the wafer
- $4$ sites (still not working): Pick a random site in four sections -- the smaller circle mentioned above and three equally-sized sections of the wafer
- $\geq5$ sites: Pick a random site in five sections -- the smaller circle mentioned above and four equally-sized quadrants of the wafer 

## Fixed Site Selection
This method has $6$ use cases. 

- $1$ site: Pick the central die of the wafer 
- $2$ sites: Pick the central die as well as the topmost die (with the same x-coordinate as the central one) 
- $3$ sites: Pick the central die, topmost die, and rightmost die (again, aligning with the central coordinate) 
- $4$ sites: Pick the central die, topmost die, rightmost die, and bottommost die 
- $5$ sites: Pick the central die, topmost die, rightmost die, bottommost die, and leftmost die 
- $>5$ sites: Pick all dies specified above and create a rough imaginary circle $3/4$ the radius of the wafer. Pick another 8 sites evenly spaced around this circle -- if there is more sites you must pick, make another circle 3/4 the size of the previous circle, and continue
