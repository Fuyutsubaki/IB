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

Design={
None=0,TriCir=1,Egg=2,Mini=3
,BoxEmy=101
}




function nway(n,ang,f)
	p=cb.ang
	for i=0,n-1 do
		cb.ang=p+ang/(n-1)*(i-n/2.)
		f()
	end
	cb.ang=p
end


function cirnway(n,f)
	local tmp=cb.ang
	local e=2*pi/n
	for i=0,n-1 do
		cb.ang=cb.ang+e
		f()
	end
	cb.ang=tmp
end



function push(f,...)
	local co=coroutine.create(f)
	coroutine.resume(co,...)
	table.insert(cb.subco,co)
	
end

function pushTime(N,p,f,...)
	push(Time,N,p,f,...)
end
Co={}
--current Bullet
cb=nil;
function Co:updata()
	cb =self
	for key,val in ipairs(self.subco)do
		if coroutine.status(val)=="dead" then
			table.remove(self.subco,key);
		end
	end
	for _,val in ipairs(self.subco)do
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


function shot(design,f)
	return regCo(cb.x,cb.y,cb.ang,design,f)
end
function shota(ang,design,f)
	return regCo(cb.x,cb.y,ang,design,f)
end
pi=math.pi
Long=99999999
function move(ang,spd)
	cb.x=cb.x+math.cos(ang)*spd
	cb.y=cb.y+math.sin(ang)*spd
end
function movein(ang,len,N)
	local a=2*len/(N^2)
	Timei(N,0,function(i)
			move(ang,a*i)
		end) 
	return 2*len/N
end
function moveout(ang,len,N)
	local a=2*len/(N^2)
	Timei(N,0,function(i) move(ang,a*(N-i))end) 
	return 0
end

function moveang(ang)
	cb.ang=cb.ang+ang
end

function aim(x,y)
	return math.atan2(y-cb.y,x-cb.x)
end
function length(x,y)
	local X=(cb.x-x)
	local Y=(cb.y-y)
	return math.sqrt(X^2+Y^2)
end


function movest(spd)
	move(cb.ang,spd)
end
function movestin(len,N)
	local a=2*len/(N^2)
	Timei(N,0,function(i) movest(a*i)end) 
	return 2*len/N
end
function movestout(len,N)
	local a=2*len/(N^2)
	Timei(N,0,
	function(i) 
		movest(a*(N-i))
		end) 
	return 0
end

function move_cir(r,spd)
	moveang(spd/r)
	movest(spd)
end

function ikarugano()
cirnway(3,
		function()
			regCo(cb.x,cb.y,cb.ang,Design.TriCir,
				function()
					Time(2,0,movest,18)
					local spd=2;
					local r=36
					push(moveout,pi,270,90)
					push(
						function()
							Timei(Long,5,
							function(i)
								shota(cb.ang+pi/360*i,Design.Mini,function()Time(Long,0,movest,1)
								end)end)
						end)
					moveang(pi/2)
					Timei(Long,0,function(i)move_cir(r,spd)end)
				
				end)
			end)
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

function w12e()
	local spd=2;
	local r=36
	push(moveout,pi,270,90)
	moveang(pi/2)
	Time(490,0,function()	moveang(spd/r);	movest(spd)end)
	Time((pi*r/(2*spd)),0,function()	moveang(2*spd/r);	movest(spd)end)
	
	Time(Long,0,function ()movest(spd)end)
end

function w11e()
	cirnway(3,
		function()
			regCo(cb.x,cb.y,cb.ang,Design.TriCir,
				function()
					Time(2,0,movest,18)
					local spd=2;
					local r=36
					push(moveout,pi,270,90)
					moveang(pi/2)
					Timei(360,0,function(i)move_cir(r,spd+i/180.)end)
					Time((pi*r/(2*spd)),0,function()	moveang(2*spd/r);	movest(spd)end)
					
					Time(Long,0,function ()movest(spd+2)end)
				end)
			end)
end



function ms1()
	push(function()
		Time(3,10,nway(8,pi*2,sinCc(3,70,Design.BoxEmy)))
	end)
	moveout(0,360,60)
end
function Main()
	regCo(400,120,0,Design.BoxEmy,ikarugano)
end
