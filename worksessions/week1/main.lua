local max_agents = 100

local agents
local overrun_count

-- https://love2d.org/wiki/HSL_color
function hsl_to_rgb(h, s, l, a)
	if s<=0 then return l,l,l,a end
	h, s, l = h*6, s, l
	local c = (1-math.abs(2*l-1))*s
	local x = (1-math.abs(h%2-1))*c
	local m,r,g,b = (l-.5*c), 0,0,0
	if h < 1     then r,g,b = c,x,0
	elseif h < 2 then r,g,b = x,c,0
	elseif h < 3 then r,g,b = 0,c,x
	elseif h < 4 then r,g,b = 0,x,c
	elseif h < 5 then r,g,b = x,0,c
	else              r,g,b = c,0,x
	end 
    return {r+m, g+m, b+m, a}
end

-- https://love2d.org/forums/viewtopic.php?t=83180
function lerp(a,b,t) return a * (1-t) + b * t end

function random_angle(scale)
    local angle_rad = math.random() * math.pi * 2
    return math.cos(angle_rad) * scale, math.sin(angle_rad) * scale
end

function random_color()
    return hsl_to_rgb(math.random(), 1, 0.7)
end

function distance(x1, y1, x2, y2)
    return math.sqrt((x2 - x1) ^ 2 + (y2 - y1) ^ 2)
end

function cross(x1, y1, x2, y2)
    return x1 * y2 - y1 * x2
end

function make_agent(x, y)
    return {
        x = x,
        y = y,
        direction = 0,
        speed = 0,
        radius = 8,
        close_proximity = 40,
        far_proximity = 90,
        cohesion_proximity = 150,
        color = {1, 0.5, 0, 1},
    }
end

function apply_separation(agent, other_agents)
    for i,other_agent in ipairs(other_agents) do
        if other_agent ~= agent then
            local other_distance = distance(agent.x, agent.y, other_agent.x, other_agent.y)
            if other_distance < agent.close_proximity then
                if cross(agent.x, agent.y, other_agent.x, other_agent.y) > 1 then
                    agent.direction = agent.direction + 0.1
                else
                    agent.direction = agent.direction - 0.1
                end
            end
        end
    end
end

function apply_alignment(agent, other_agents)
    for i,other_agent in ipairs(other_agents) do
        if other_agent ~= agent then
            if other_distance < agent.far_proximity then
                alignment_sum = alignment_sum + other_agent.direction
                aligning_neighbors = aligning_neighbors + 1
            end
        end
    end
end

function update_agent(agent, dt)
    agent.x = agent.x + math.cos(agent.direction) * agent.speed * dt
    agent.y = agent.y + math.sin(agent.direction) * agent.speed * dt

    agent.x = agent.x % love.graphics.getWidth()
    agent.y = agent.y % love.graphics.getHeight()

    -- DUMB! O(n^2)!
    local neighbors = {}
    local alignment_sum = 0
    local aligning_neighbors = 0
    local cohesion_avg_x = 0
    local cohesion_avg_y = 0
    local cohesion_neighbors = 0
    for i,other_agent in ipairs(agents) do
        if other_agent ~= agent then -- don't check against self
            
            if other_distance < agent.cohesion_proximity then
                cohesion_avg_x = cohesion_avg_x + other_agent.x
                cohesion_avg_y = cohesion_avg_y + other_agent.y
                cohesion_neighbors = cohesion_neighbors + 1
            end
        end
    end

    if cohesion_neighbors > 0 then
        local target_dir = math.atan((cohesion_avg_y/cohesion_neighbors - agent.x), (cohesion_avg_x/cohesion_neighbors - agent.y))
        agent.direction = lerp(agent.direction, target_dir, 0.02)
    end
    
    if aligning_neighbors > 0 then
        agent.direction = lerp(agent.direction, alignment_sum / aligning_neighbors, 0.2)
    end
end

function love.load()
    -- print("love.load")
    agents = {}
    for i=1,max_agents do
        local agent = make_agent(64, 64)
        agent.x = love.math.random(0, love.graphics.getWidth())
        agent.y = love.math.random(0, love.graphics.getHeight())
        agent.direction = math.random() * math.pi * 2 -- tau :)
        agent.speed = love.math.random(50, 200)
        agent.color = {1,1,1,1}
        table.insert(agents, agent)
    end

    overrun_count = 0
end

function love.update(dt)
    for i,agent in ipairs(agents) do
        update_agent(agent, dt)
    end

    local fps = 1/dt
    love.window.setTitle(tostring(fps))
    if fps < 50 then overrun_count = overrun_count + 1 end
end

function love.draw()
    love.graphics.clear()
    for i,agent in ipairs(agents) do
        if i <= 3 then
            love.graphics.setColor(0,1,0,1)
            love.graphics.circle("line", agent.x, agent.y, agent.close_proximity)
            love.graphics.circle("line", agent.x, agent.y, agent.far_proximity)
            local offset_x = math.cos(agent.direction) * 20
            local offset_y = math.sin(agent.direction) * 20
            love.graphics.line(agent.x, agent.y, agent.x + offset_x, agent.y + offset_y)
        end

        love.graphics.setColor(agent.color)
        love.graphics.circle("fill", agent.x, agent.y, agent.radius)
    end
    love.graphics.setColor(1, 0, 0, 1)
    love.graphics.print(tostring(overrun_count), 10, 10)
end