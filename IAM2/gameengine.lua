shotStack={}
setStack={}
--current Bullet
cb=nil;
--current thread
ct=nil;
pi=math.pi
Long=99999999
DTriCir=1;DEgg=2;DMini=3
DEBox=101
MinX=0;MaxX=960;MinY=0;MaxY=540;
playerX=0;playerY=0;

function regBt(x,y,ang,design)
	local bt={}
	bt.x=x
	bt.y=y
	bt.ang=ang
	bt.design=design
	bt.setter={}
	table.insert(shotStack,bt)
	return bt
end
function regEnemy(x,y,ang,design,hp)
	local bt={}
	bt.x=x
	bt.y=y
	bt.ang=ang
	bt.design=design
	bt.seter={}
	bt.hp=hp;
	table.insert(setStack,bt)
	return bt
end

function frameInit(x,y)
	playerX=x
	playerY=y
	shotStack={}
	setStack={}
end
Co={}
function Co:updata()
	cb =self
	for key,val in ipairs(self.co)do
		if coroutine.status(val.f)=="dead" then
			table.remove(self.co,key);
		end
	end
	for _,val in ipairs(self.co)do
		ct=val
		coroutine.resume(val.f)
	end
	return self.x , self.y ,self.ang,self.design
end

function regCo(x,y,ang,design,f)
	local bt=regBt(x,y,ang,design)
	bt.co={{f=coroutine.create(f),setter={}}}
	bt.updata=Co.updata;
	return bt
end
function regCoE(x,y,ang,design,f,hp)
	local bt=regEnemy(x,y,ang,design,hp)
	bt.co={{f=coroutine.create(f),setter={}}}
	bt.updata=Co.updata
	return bt
end


function set()
	ct.setter.set()
end
function seta(ang)
	local tmp=cb.ang
	cb.ang=ang;
	ct.setter.set()
	cb.ang=tmp;
end
-------------------------------------------------------------
function setBt(design,f)
	ct.setter={
		design=design,
		f=f,
		set=function()regCo(cb.x,cb.y,cb.ang,ct.setter.design,ct.setter.f)end}
end

function setE(hp,design,f)
	ct.setter={design=design,f=f,hp=hp,set=function()regCoE(cb.x,cb.y,cb.ang,ct.setter.design,ct.setter.f,ct.setter.hp)end}
end

function sleep(n)
	for i=0,n-1 do
		coroutine.yield();
	end
end
function Timei(n,p,func)
	for i=0,n-1 do
		func(i)
		coroutine.yield();
		sleep(p)
	end
end
function Time(n,p,f,...)
	for i=0,n-1 do
		f(...)
		coroutine.yield();
		sleep(p)
	end
end


function push(f)
	local co=coroutine.create(f)
	table.insert(cb.co,{f=co,setter={}})
end

function pushTime(N,p,f)
	push(Time,N,p,f)
end



function jump(x,y)
	cb.x=x;
	cb.y=y;
end
