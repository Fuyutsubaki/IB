require("gameengine")
function nway(n,ang)
	local tmp=cb.ang
	for i=0,n-1 do
		cb.ang=tmp+(ang/(n-1))*(i-(n-1)/2.)
		set()
	end
	cb.ang=tmp
end


function cirnway(n)
	local tmp=cb.ang
	local e=2*pi/n
	for i=0,n-1 do
		cb.ang=cb.ang+e
		set()
	end
	cb.ang=tmp
end



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

function movestXYBt(x,y)
	return function()
		Time(Long,0,function()cb.x=cb.x+x;cb.y=cb.y+y;end)
	end
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
	setE(300)
	cirnway(3)


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




function wav13()
	local enemy=function ()
		push(function()setBt(DTriCir,movestXYBt(3,4));Time(300,10,set)end)
		Time(Long,0,movest,3);
	end
	setE(300,DEBox,enemy)
	set()
end









function Main()
	regCo(120,120,0,DEBox,wav13)
end
