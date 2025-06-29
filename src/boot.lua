function axo.boot()
  print("booting axo...")

  require("axo.filesystem")
end

function axo.init()
  print("initializing axo...")
end

function axo.run()
  print("running axo...")

  return function()
    print("main loop running...")
    return 0
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
