require 'redcarpet'
require 'rouge'
require 'rouge/plugins/redcarpet'

class HTML < Redcarpet::Render::HTML
  include Rouge::Plugins::Redcarpet
end

class ChalkTheme < Rouge::Themes::Base16
  name 'base16.chalk'
  light!

  palette base00: "#151515"
  palette base01: "#202020"
  palette base02: "#303030"
  palette base03: "#505050"
  palette base04: "#b0b0b0"
  palette base05: "#d0d0d0"
  palette base06: "#e0e0e0"
  palette base07: "#f5f5f5"
  palette base08: "#fb9fb1"
  palette base09: "#eda987"
  palette base0A: "#ddb26f"
  palette base0B: "#acc267"
  palette base0C: "#12cfc0"
  palette base0D: "#6fc2ef"
  palette base0E: "#e1a3ee"
  palette base0F: "#deaf8f"
end

css = <<-CSS
@import url('https://fonts.googleapis.com/css?family=Fira+Mono');

code {
  font-family: 'Fira Mono', monospace;
  font-size: 14pt;
}

#{ChalkTheme.mode(:light).render(scope: ".highlight")}
CSS

md = File.read("CONTENT.md")

Dir["steps/*"].each do |step|
  if step =~ /^steps\/\d+\-([\w-]+)$/
    name = $1
    if name == "makefile"
      code = File.read("#{step}/Makefile")
      lang = "make"
    else
      code = File.read("#{step}/kilo.c")
      lang = "c"
    end
    code.gsub! '\\', '\\\\\\\\'
    md.sub! "{{#{name}}}", "```#{lang}\n#{code}```"
  end
end

markdown = Redcarpet::Markdown.new(HTML, fenced_code_blocks: true)
doc = markdown.render(md)

html = <<-HTML
<!doctype html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Build Your Own Text Editor</title>
    <link href="style.css" rel="stylesheet">
  </head>
  <body>
    #{doc}
  </body>
</html>
HTML

File.write("buildYourOwnTextEditor.html", html)
File.write("style.css", css)

