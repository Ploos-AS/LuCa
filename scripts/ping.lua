function on_irc(msg)
  if msg.command == "PRIVMSG" and msg.params[2] == "!ping" then
    luca.send("PRIVMSG " .. msg.params[1] .. " :pong")
  end
end
