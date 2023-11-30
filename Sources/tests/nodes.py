Lx = 0.1	
Ly = 0.024
Lz = 0.012
dx = 0.006
r = dx /2.0
h = dx * 1.2

nel = [0,0,0]

# elnod = (100,8)

file = open("out.txt","w")
x = y = z = 0.0
n=1
for k in range (1,6):
  z +=dx
  for k in range (1,6):
    y +=dx
    for k in range (1,6):
      file.write("model.createNode(%d, %.6e, %.6e %.6e);\n" % (n,x,y,z))
      x+=dx
      n+=1

nel[0] = int(Lx/dx) 
nel[1] = int(Ly/dx) 
nel[2] = int(Lz/dx) 
e = 0
ez = 0
i = 1
nnodz = (nel[1]+1)*(nel[2]+1)
# print("Element Nodes at z \n" % nnodz)
while ( ez < nel[2]):
  ey = 0    
  while ( ey < nel[1]):
    ex = 0
    while (ex < nel[0]): 
          # !elem%elnod(i,:)=[(nel(1)+1)*(ey+1)+ex+2,(nel(1)+1)*(ey+1)+ex+1,(nel(1)+1)*ey + ex+1,(nel(1)+1)*ey + ex+2]       
          # elnod(i,1:8) = { nnodz*ez + (nel(1)+1)*ey + ex+1,nnodz*ez + (nel(1)+1)*ey + ex+2, 
                              # nnodz*ez + (nel(1)+1)*(ey+1)+ex+2,nnodz*ez + (nel(1)+1)*(ey+1)+ex+1, 
                              # nnodz*(ez + 1) + (nel(1)+1)*ey + ex+1,nnodz*(ez + 1) + (nel(1)+1)*ey + ex+2, 
                              # nnodz*(ez + 1) + (nel(1)+1)*(ey+1)+ex+2,nnodz*(ez + 1)+ (nel(1)+1)*(ey+1)+ex+1}
        file.write("model.createElement(%d %d %d %d %d %d %d);\n" % (i, nnodz*ez+ (nel[0]+1)*ey + ex+1,   nnodz*ez + (nel[0]+1)*ey + ex+2, \
                                                                  nnodz*ez + (nel[0]+1)*(ey+1)+ex+2,      nnodz*ez + (nel[0]+1)*(ey+1)+ex+1,\
                                                                  nnodz*(ez + 1) + (nel[0]+1)*ey + ex+1,  nnodz*(ez + 1) + (nel[0]+1)*ey + ex+2)
                  )
        # printf("Element , Elnod" %(i,elem%elnod(i,:))) 
        i=i+1
        ex = ex + 1
    ey = ey + 1
  ez = ez + 1
