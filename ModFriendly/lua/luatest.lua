-- simple test program to demonstrate mod-friendly design
-- see accompanying documentation for more information
--define some global variables
somenumber = 100
someothernumber = 200

function sum(a,b)
	return a + b
end

function callCPP()
--The CPP function pushes "(OK)" onto stack == returns "(OK)" when called from here
	print(CPPFunction())
end

function callPureCPP()
	PureCPPFunction()
end