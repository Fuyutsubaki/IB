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
function calAngSpd(r,spd)
	
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
function movestVBt(v)
	return function()
		Time(Long,0,function()movest(v)end)
	end
end
function getAim()
	return math.atan2(playerY-cb.y,playerX-cb.x);
end


