function countingForLoop(a,b)
  for i=a,b do
    print(i)
  end
end

function testPrintFunc(a)
  print("testPrintFunc: " .. a)
end

countingForLoop(1,10)
testPrintFunc("hello") -- prints hello
print("1: "..(5==10 and "y" or "z")) -- prints z (the false part of the 'or')
print("2: "..(5==5 and "y" or "z")) -- prints y (the true part of the 'or')
print("3: "..(5==5 and "kt")) -- prints 'k'
print("4: "..tostring((5==6 and "k"))) -- prints 'false' as no second part of the 
print("5: "..tostring((5==5))) -- prints true

-- Experimenting with tables...
months=
{
  jan=1,
  feb=2,
  mar=3,
  apr=4,
  may=5,
  jun=6,
  jul=7,
  aug=8,
  sep=9,
  oct=10,
  nov=11,
  dec=12
};

print(months["feb"])
assert(months["jan"]==1)
assert(months["oct"]==10)