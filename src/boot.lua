local function get_low_arg(a)
  local m = math.huge
  for k,v in pairs(a) do
    if k < m then
      m = k
    end
  end
  return a[m], m
end

function axo.boot()
  print("booting axo...")

  require("axo.filesystem")

  local arg0 = get_low_arg(arg)
  axo.filesystem.init(arg0)

  axo.filesystem.set_identity("test")
  axo.filesystem.set_source("test")

  require("main")
end

function axo.init()
  print("initializing axo...")

  require("axo.event")
end

function axo.run()
  print("running axo...")

  if axo.load then axo.load() end

  if axo.timer then axo.timer.step() end

  return function()
    if axo.event then
      axo.event.pump()

      while true do
        local name, a,b,c,d,e,f = axo.event.poll()
        if not name then break end

        if name == "quit" then
          if not axo.quit or not axo.quit() then
            return a or 0
          end
        end

        if axo.handlers[name] then
          axo.handlers[name](a,b,c,d,e,f)
        end
      end
    end

    local dt = axo.timer and axo.timer.step() or 0

    if axo.update then axo.update(dt) end

    if axo.graphics and axo.graphics.isActive() then
      axo.graphics.clear(axo.graphics.getBackgroundColor())

      if axo.draw then axo.draw() end

      axo.graphics.present()
    end

    if axo.timer then axo.timer.sleep(0.001) end
  end
end

local function error_printer(msg, layer)
  print((debug.traceback("error: " .. tostring(msg), 1 + (layer or 1)):gsub("\n[^\n]+$", "")))
end

return function()
  local func
  local inerror = false

  local function defer_errhand(...)
    local errhand = axo.errorhandler or axo.errhand
    local handler = (not inerror and errhand) or error_printer
    inerror = true
    func = handler(...)
    inerror = false
  end

  local function earlyinit()
    func = nil

    -- if boot fails, finish immediately
    local result = xpcall(axo.boot, error_printer)
    if not result then return 1 end

    -- if init or run fail, the error handler will take over
    result = xpcall(axo.init, defer_errhand)
    if not result then return end

    local main
    result, main = xpcall(axo.run, defer_errhand)
    if result then
      func = main
    elseif inerror then -- error in error handler
      print("error: " .. tostring(main))
    end
  end

  func = earlyinit

  while func do
    local _, retval = xpcall(func, defer_errhand)
    if retval then return retval end
    coroutine.yield()
  end

  return 1
end
