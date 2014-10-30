shotStack={}

function regBt(x,y,ang,design)
	local bt={}
	bt.x=x
	bt.y=y
	bt.ang=ang
	bt.design=design
	table.insert(shotStack,bt)
	return bt
end

function frameInit()
	shotStack={}

end

function sleep(n)
	for i=0,n do
		coroutine.yield();
	end
end
function Time(n,p,func)
	for i=0,n-1 do
		func()
		sleep(p)
	end
end
Design={None=0,TriCir=1,Egg=2}

function nway(n,ang,f)
	return function()
		p=cb.ang
		for i=0,n-1 do
			cb.ang=p+ang/(n-1)*(i-n/2.)
			f()
		end
		cb.ang=p
	end
end

Co={}
--current Bullet
cb=nil;
function Co:updata()
	cb =self
	for key,val in ipairs(self.subco)do
		coroutine.resume(val,self)
	end
	coroutine.resume(self.co,self)
	return self.x , self.y ,self.ang,self.design
end

function regCo(x,y,ang,design,f)
	local bt=regBt(x,y,ang,design)
	bt.subco={}
	bt.co=coroutine.create(f)
	bt.updata=Co.updata
	return bt
end

pi=math.pi

function movest(x,y)
	return function ()
		cb.x=cb.x+x
		cb.y=cb.y+y
	end
end
function Co:sin()
	while true do
		self.count=self.count+1
		self.ang = self.ang0 + math.sin(self.count*math.pi/self.per +self.tr)*math.pi/4
		self.x=self.x+math.cos(self.ang)*self.speed
		self.y=self.y+math.sin(self.ang)*self.speed
		coroutine.yield();
	end
end

function sinCc(speed,per,design)
	return function()
		local bt=regCo(cb.x,cb.y,cb.ang,design,Co.sin)
		bt.count=0
		bt.tr=math.pi/2
		bt.per=per
		bt.speed=speed
		bt.ang0=cb.ang
		
		local bt2=regCo(cb.x,cb.y,cb.ang,design,Co.sin)
		bt2.count=0
		bt2.tr=-math.pi/2
		bt2.per=per
		bt2.speed=speed
		bt2.ang0=cb.ang
	end
end


function ms1()
	Time(10,2,	
		function()
			nway(8,pi*2,sinCc(3,70,Design.TriCir))()
			cb.x=cb.x+5;
		end
	)
	cb.x=cb.x+7;
end
function Main()
	regCo(120,120,pi/4,Design.TriCir,ms1)
end
