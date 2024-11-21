from z3 import *


solver = Solver()

pin = [Int(f'digit_{i}') for i in range(11)]

for digit in pin:
    solver.add(digit >= 0, digit <= 9)


solver.add(pin[2] % 3 == 1)                       
solver.add(pin[1] * pin[9] == 12)                 
solver.add(pin[3] * 2 == pin[4])                  
solver.add(pin[5] + pin[1] == 9)                  
solver.add(pin[6] % 2 == 0)                       
solver.add(pin[0] + pin[10] == 10)                
solver.add(pin[7] + pin[2] == pin[6])             
solver.add(pin[8] - pin[0] == 1)                  
solver.add(pin[9] * 3 == pin[10])                 
solver.add(pin[10] % 2 == 0)                      


if solver.check() == sat:
    
    print("Possible PINs:")
    while solver.check() == sat:
        model = solver.model()
        pin_solution = [model[digit].as_long() for digit in pin]
        print("".join(map(str, pin_solution)))
        
       
        solver.add(Or([digit != model[digit] for digit in pin]))
else:
    print("No solution found")
