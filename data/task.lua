----------------任务1--------------
function task1(state,index)         
    if(state==0) then
         if(index==0) then
                  CloseTalk()
         elseif(index==1) then
                 SetNpcTalk("这位先生!\n不知道你是否有空，可以帮个忙\n好吗？","好的,请问是什么事呢","不好意思，我还有事在身")
         elseif(index==2) then
                SetNpcTalk("燃烧军团已经毁灭了村庄\n，你能消灭他们吗？","竟然有这样的事，我就去帮忙","燃烧军团我也对付不\n了，对不起啊")
         elseif(index==3) then
                EndTalk();
                CloseTalk()
         end
   elseif(state==1) then
         if(index==0) then
                 CloseTalk()
         elseif(index==1) then
                 SetNpcTalk("燃烧军团严重影响了村民\n的生活，先生还是早点\n行的为好","好的","返回")
         elseif(index==2) then
                 EndTalk();
                 CloseTalk()
         end
   elseif(state==2) then
         if(index==0) then
                 CloseTalk()
         elseif(index==1) then
                   SetNpcTalk("先生果然是个能人，这么快就\n解决燃烧军团，我代表所有\n村民感谢你","不必客气，我也是尽我\n所能罢了！","返回")
          elseif(index==2) then
                   EndTalk();
                   CloseTalk()
       end
  end
end