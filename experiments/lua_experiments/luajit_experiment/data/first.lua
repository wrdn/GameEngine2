function countingForLoop(a,b)
  for i=a,b do
    print(i)
  end
end

function testPrintFunc(a)
  print("testPrintFunc: " .. a)
end

countingForLoop(1,10)
testPrintFunc("hello")