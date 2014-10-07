function regBt(x,y,ang)
	local bt=_regBt(x,y)
	bt.x=x
	bt.y=y
	bt.ang=ang
	return bt
end

Co={}
function Co:updata()
	coroutine.resume(self.co,self)
	return self.x , self.y
end

PI=math.Pi

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

function Main()
	sinBt(120,120,0,2,40)
	rsinBt(120,120,0,2,40)
end
stBt={}
function stBt:updata()
	self.x=self.vx+self.x
	self.y=self.vy+self.y
	
end


