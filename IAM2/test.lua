function regBt(x,y,ang)
	local bt=_regBt(x,y)
	bt.x=x
	bt.y=y
	bt.ang=ang
	return bt
end

function sleep(n)
	for i=0,n do
		coroutine.yield();
	end
end
function Time(n,p,func)
	for i=0,n do
		func()
		sleep(p)
	end

end

Co={}
--current Bullet
cb=nil;
function Co:updata()
	cb =self
	coroutine.resume(self.co,self)
	return self.x , self.y
end

function regCo(x,y,ang,f)
	local bt=regBt(x,y,ang)
	bt.co=coroutine.create(f)
	bt.updata=Co.updata
end
PI=math.Pi
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

function sinBt(x,y,ang,speed,per)
	local bt=regBt(x,y,ang)
	bt.count=0
	bt.tr=math.pi/2
	bt.per=per
	bt.speed=speed
	bt.ang0=ang
	bt.co=coroutine.create(Co.sin)
	bt.updata=Co.updata
end
function rsinBt(x,y,ang,speed,per)
	local bt=regBt(x,y,ang)
	bt.count=0
	bt.tr=-math.pi/2
	bt.per=per
	bt.speed=speed
	bt.ang0=ang
	bt.co=coroutine.create(Co.sin)
	bt.updata=Co.updata
end


function Co:test()
	Time(30,0,movest(3,3))
	Time(30,0,movest(0,3))
	Time(30,0,movest(3,0))
end
function Main()
	regCo(0,0,0,Co.test)
end
