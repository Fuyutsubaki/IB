require("gameengine")
require("mover")
function madarabato()
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
	
end
function ms1()
	push(function()
		Time(3,10,nway(8,pi*2,sinCc(3,70,Design.BoxEmy)))
	end)
	moveout(0,360,60)
end
function test()
	local f=function()
		
		push(function()
			setBt(DMini ,function()Time(Long,0,movest,3)end)
			Time(Long,30,nway,5,pi)
		end)
		Time(Long,0,movest,3)
	end
	setE(300,DEBox,f)
	set()
	sleep(30)
	set()
	sleep(30)
	set()
end

function last_dance()
	local enemy=function(N)
		movein(getAim()+pi/12,getLen()/4,N)
		sleep(N)
		local a=getAim()
		local len=getLen()
		setBt(DEgg,movestVBt(len/N));
		Time(10,8,seta,a)
		Time(Long,0,move,a+pi,4)
	end
	
	local lam=function(N)
		local rand=math.random(MinY,MaxY)
		jump(MinX,rand)
		setE(12,DEBox,function()enemy(N)end)
		set()
		rand=math.random(MinY,MaxY)
		jump(MaxX,rand)
		setE(12,DEBox,function()enemy(N)end)
		set()
		rand=math.random(MinX,MaxX)
		jump(rand,MinY)
		setE(12,DEBox,function()enemy(N)end)
		set()
		rand=math.random(MinX,MaxX)
		jump(rand,MaxY)
		setE(12,DEBox,function()enemy(N)end)
		set()
	end
	
	for i=0,14 do
		lam(100-i*5)
		sleep((100-i*5)*3)
	end
		for i=0,15 do
		lam(30-i/3)
		sleep((30-i/3)*3)
	end

end

function osakanass()
	local enemy=function (v)
		return function()push(function()setBt(DTriCir,movestXYBt(-3,v));Time(300,10,set)end)
		Time(Long,0,move,pi,3);end
	end
	local enemy2=function(v)
			return function()
			
			push(function() setE(300,DEBox,enemy(v));Time(Long,30,set)end)
			Time(360,0,function()move(0,2)end)
			jump(Long,Long)
			end
	end
	
	setE(12,DEBox,enemy2(4))
	jump(200,MinY)
	set()
	setE(12,DEBox,enemy2(-4))
	jump(200,MaxY )
	set()
	sleep(720)
end


function BlackPenis()
	local enemy=function (v)
		return function()push(function()setBt(DTriCir,movestXYBt(3,v));Time(300,10,set)end)
		Time(Long,0,movest,3);end
	end
	local enemy2=function(v)
			return function()
			setE(300,DEBox,enemy(v))
			Time(15,30,set)
			jump(Long,Long)
			end
	end
	local enemy3=function()
		push(function()setBt(DEgg,movestXYBt(0,3));Time(Long,20,set)end);
		push(function()setBt(DEgg,movestXYBt(0,-3));Time(Long,20,set)end);
		Time(Long,0,move,pi,3)
	end
	local enemy4=function()
		setE(250,DEBox,enemy3)
		Time(7,15,set)
		jump(Long,Long)
	end
	
	
	
	
	setE(12,DInv,enemy2(4))
	jump(MinX,MinY)
	set()
	setE(12,DInv,enemy2(-4))
	jump(MinX,MaxY )
	set()
	jump(MaxX-30,360 )
	setE(12,DInv,enemy4)
	set()
	sleep(680)
end


function Imalice()
	local enemy=function(a,len ,lr)
		return function()
			movest(36)
			local spd=2;
			local r=36
			push(function()sleep(90);setBt(DMini,movestVBt(3)) Time(10,30,set)end)
			push(function()moveout(a,len,90) end)
			moveang(pi/2)
			Time(360,0,function()move_cir(60,3*lr)end)
			Time((pi*r/(2*spd)),0,function()	moveang(2*spd/r);	movest(spd)end)
			Time(Long,0,function ()movest(spd+2)end)
		end
	end
	
	jump(MaxX,270)
	setE(MaxX,DEBox ,enemy(pi,270 ,1))
	cirnway(3)
	
	sleep(60)
	jump(MinX,360)
	setE(300,DEBox ,enemy(0,180,-1))
	cirnway(3)
	sleep(60)
	jump(MaxX,100)
	setE(300,DEBox ,enemy(pi,180,1))
	cirnway(3)
end
function Imalice2()
	local enemy=function(a,len ,lr,s)
		return function()
			local r=48
			movest(r)
			local spd=2;
			
			push(function()
				sleep(90);Time(3,60,function() for i=3,6 do setBt(DMini,movestXYBt(i/-2,0));set() end end)
				end)
			push(function()setBt(DTriCir,movestVBt(3));sleep(90);Time(3,90,function()seta(getAim())end)end)
			push(function()moveout(a,len,s) end)
			moveang(pi/2)
			Time(360,0,function()move_cir(r,2*lr)end)
			Time((pi*r/(2*spd)),0,function()	moveang(2*spd/r);	movest(spd)end)
			Time(Long,0,function ()movest(spd+2)end)
		end
	end
	
	jump(MaxX,270)
	setE(MaxX,DEBox ,enemy(pi,270 ,1,90))
	cirnway(3)
	
	sleep(600)
	
	jump(MaxX,270)
	setE(300,DEBox ,enemy(pi,100 ,1,60))
	cirnway(3)
	
	sleep(180)
	jump(MaxX,360)
	setE(200,DEBox ,enemy(pi,480 ,1,150))
	cirnway(3)
	sleep(600)
end

function dynam()

	local enemy=function(a)
		
		
		push(function()setBt(DEgg,movestXYBt(0,3));Time(Long,10,set)end)
		Time(Long,0,move,a,6)
	end
	local enemy2=function(a)
		setE(500,DEBox,function()enemy(a)end)
		Time(5,13,set)
		jump(Long,Long)
	end
	local benri=function(h)
		setE(12,DInv,function()enemy2(0)end)
		jump(MinX,h)
		set()
		setE(12,DInv,function()enemy2(pi)end)
		jump(MaxX,h)
		set()
		sleep(90)
	end
	
	benri(360)
	benri(240)
	benri(120)
	benri(30)
end



function game()
	Imalice2()
	Imalice()
	osakanass()
	
	BlackPenis()
	dynam()
	last_dance()
end

function Main()
	regCo(120,120,0,DInv,game)
end
