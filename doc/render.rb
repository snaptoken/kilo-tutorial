require 'fileutils'
require 'rugged'
require 'redcarpet'
require 'rouge'

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

FileUtils.rm_rf("html_out")
FileUtils.mkdir("html_out")

Dir["html_in/*"].each do |f|
  name = File.basename(f)
  unless %w(template.html style.css).include? name
    FileUtils.cp(f, "html_out/#{name}")
  end
end

html_template = File.read("html_in/template.html")
css = File.read("html_in/style.css")
css << ChalkTheme.mode(:light).render(scope: ".highlight")

File.write("html_out/style.css", css)

diffs = {}
FileUtils.cd("../steps") do
  FileUtils.mkdir("0")

  last_step = "0"
  Dir["*"].sort_by(&:to_i).each do |step|
    names = [step.to_i.to_s]
    if step =~ /\d+\-([\w-]+)$/
      names << $1
    end

    html = ""
    in_diff = false
    lexer = nil
    formatter = Rouge::Formatters::HTML.new

    diff = `git diff --histogram --unified=100000 --no-index #{last_step} #{step}`
    diff.lines.each do |line|
      if !in_diff && line =~ /^\+\+\+ (.+)$/
        if html.length > 0
          raise NotImplementedError
        end
        filename = File.basename($1)
        lexer = Rouge::Lexer.guess(filename: filename)
        html << "<div class=\"diff\">\n"
        html << "<div class=\"filename\">#{filename}</div>\n"
        html << "<pre class=\"highlight\"><code>"
      elsif line.start_with? '@@'
        in_diff = true
      elsif in_diff && [' ', '+', '-'].include?(line[0])
        hl = formatter.format(lexer.lex(line[1..-1])).gsub("\n", "")
        klass = {' ' => :nochange, '+' => :add, '-' => :remove }[line[0]]
        html << "<div class=\"line #{klass}\">#{hl}</div>"
      else
        in_diff = false
      end
    end
    html << "</code></pre>\n</div>\n"

    names.each do |name|
      diffs[name] = html
    end

    last_step = step
  end

  FileUtils.rmdir("0")
end

index = ""
markdown = Redcarpet::Markdown.new(Redcarpet::Render::HTML)
Dir["*.md"].sort.each do |md_file|
  html_file = md_file.sub(/\.md$/, '.html')

  md = File.read(md_file)
  md =~ /^# (.+)$/
  title = $1

  index << "<li><a href='#{html_file}'>#{title}</a></li>\n"

  content = markdown.render(md)
  content.gsub!(/<p>{{([\w-]+)}}<\/p>/) { diffs[$1] }

  html = html_template.dup
  html.gsub!("{{title}}") { title }
  html.gsub!("{{content}}") { content }

  File.write(File.join("html_out", html_file), html)
end

content = <<-HTML
<h1>Build Your Own Text Editor</h1>
<h2>Table of Contents</h2>
<ol>
  #{index}
</ol>
HTML

html = html_template.dup
html.gsub!("{{title}}", "Build Your Own Text Editor")
html.gsub!("{{content}}", content)

File.write("html_out/index.html", html)

