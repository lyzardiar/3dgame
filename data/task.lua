----------------����1--------------
function task1(state,index)         
    if(state==0) then
         if(index==0) then
                  CloseTalk()
         elseif(index==1) then
                 SetNpcTalk("��λ����!\n��֪�����Ƿ��пգ����԰��æ\n����","�õ�,������ʲô����","������˼���һ���������")
         elseif(index==2) then
                SetNpcTalk("ȼ�վ����Ѿ������˴�ׯ\n����������������","��Ȼ���������£��Ҿ�ȥ��æ","ȼ�վ�����Ҳ�Ը���\n�ˣ��Բ���")
         elseif(index==3) then
                EndTalk();
                CloseTalk()
         end
   elseif(state==1) then
         if(index==0) then
                 CloseTalk()
         elseif(index==1) then
                 SetNpcTalk("ȼ�վ�������Ӱ���˴���\n����������������\n�е�Ϊ��","�õ�","����")
         elseif(index==2) then
                 EndTalk();
                 CloseTalk()
         end
   elseif(state==2) then
         if(index==0) then
                 CloseTalk()
         elseif(index==1) then
                   SetNpcTalk("������Ȼ�Ǹ����ˣ���ô���\n���ȼ�վ��ţ��Ҵ�������\n�����л��","���ؿ�������Ҳ�Ǿ���\n���ܰ��ˣ�","����")
          elseif(index==2) then
                   EndTalk();
                   CloseTalk()
       end
  end
end