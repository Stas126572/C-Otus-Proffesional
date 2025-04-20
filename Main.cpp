// ConsoleApplication174.cpp : Ýòîò ôàéë ñîäåðæèò ôóíêöèþ "main". Çäåñü íà÷èíàåòñÿ è çàêàí÷èâàåòñÿ âûïîëíåíèå ïðîãðàììû.
//

#include <iostream>
#include <list>
#include <fstream>

/*!
\file
\brief Struct of graphic editor
\authors Stanislav Manichev
\todo Put some code in methods
\version 0.1
*/

namespace GraphicEditor
{

    /*!
    \brief Use it to appended primitive
    \todo Append some logics
    */
    namespace Primitives
    {
        class BasicPrimitive
        {
        public:
            BasicPrimitive() {}

            virtual void draw() {};
        };

        class Triangle : public BasicPrimitive
        {
        public:
            void draw() override
            {
                std::cout << "Triangle draw" << std::endl;
            };
        };

        class Rectangle : public BasicPrimitive
        {
        public:
            void draw() override
            {
                std::cout << "Rectangle draw" << std::endl;
            };
        };
    }


    namespace Documents
    {
        struct Document
        {
            Document() {};

            std::list< Primitives::BasicPrimitive*> ls_of_pr;
            //!Appended primitive
            void add_data(Primitives::BasicPrimitive* data) { ls_of_pr.push_back(data); };

            void draw_it() {

                for (auto i : ls_of_pr)
                {
                    i->draw();
                }
            }
        };
    }

    /*!
    \brief Use it to decode document to bin
    \warning This class can't write information to file
    \todo Append some types of files
    */

    namespace Writers
    {
        template<typename FileSystem, typename BytesType>
        struct File
        {
            BytesType bt;
            FileSystem& fs;

            File(FileSystem& fs, BytesType bt) : fs(fs), bt(bt)
            {
                std::cout << "I write to std::fstream " << bt;
            }

            //! Put class of namespace TypeOfFiles to me
            template<typename DocumentType>
            void ImportToMe(std::shared_ptr<DocumentType> sh)
            {
                fs << sh.bt;
            }

            //! Get class of namespace TypeOfFiles from me
            template<typename DocumentType>
            std::shared_ptr<DocumentType> ExportFromMe()
            {
                BytesType a;
                fs >> a;
                std::shared_ptr<DocumentType> sh;
                sh->bt = a;
                return sh;
            }
        };
    }

    namespace TypeOfFiles
    {
        template<typename DocumentType, typename BytesType>
        struct BasicCLass
        {
        public:
            std::shared_ptr<DocumentType> sh_ptr; ///< Using it to get inforamtion about file
            BytesType bt;

            virtual void decode(std::shared_ptr<DocumentType> shr_ptr) //It decode sh_ptr to bytes
            {
                sh_ptr = shr_ptr;
                std::cout << "Decode document" << std::endl;
                
            }; ///< This function get information from document and put this information to file
        };
    }


    /*!
    \brief Use it to write bin document to file
    \warning It don't work with GUI document
    \todo Appended some logics
    */


    /*!
    \brief This namespace have got some types of primitive and you can appended your primitive
    \todo Appended some primitives
    */





    /*!
    \brief Use it to appended graphic primitive
    \warning This class not decode this information to document
    \todo Appended somethink new
    */
    namespace GUI
    {

        template<typename DocumentType>
        class BasicCLass
        {
            std::shared_ptr<DocumentType> sh_ptr;

        public:

            BasicCLass() {};

            BasicCLass(std::shared_ptr<DocumentType> dc) { put_document(dc); };

            //! Use it to get class from me(at namespace Documents) to get it to Writers
            std::shared_ptr<DocumentType> take_document()
            {
                return sh_ptr;
            };

            //! Use it to put class from namespace Documents(What you include from Writers) to GUI
            void put_document(std::shared_ptr<DocumentType> dc)
            {
                sh_ptr = dc;
            };

            void add_primitive(Primitives::BasicPrimitive* sh)
            {
                sh_ptr->add_data(sh);
            };

            void draw()
            {
                sh_ptr->draw_it();
            }
        };

    }


    //! Examples
    //! \warning Not use it

}

using namespace GraphicEditor;

int main()
{
    Primitives::Triangle tr;
    Primitives::BasicPrimitive* bp = static_cast<Primitives::BasicPrimitive*>(&tr);
    Documents::Document dc;
    

    GUI::BasicCLass<Documents::Document> bs;
    bs.put_document(std::make_shared<Documents::Document>(dc));
    bs.add_primitive(bp);
    bs.draw();
    auto document_ = bs.take_document();
    TypeOfFiles::BasicCLass<Documents::Document, char*> tf;
    tf.decode(document_);

    std::fstream fs("");
    Writers::File<std::fstream, char*> file_wr(fs, tf.bt);
    

}

// Çàïóñê ïðîãðàììû: CTRL+F5 èëè ìåíþ "Îòëàäêà" > "Çàïóñê áåç îòëàäêè"
// Îòëàäêà ïðîãðàììû: F5 èëè ìåíþ "Îòëàäêà" > "Çàïóñòèòü îòëàäêó"

// Ñîâåòû ïî íà÷àëó ðàáîòû 
//   1. Â îêíå îáîçðåâàòåëÿ ðåøåíèé ìîæíî äîáàâëÿòü ôàéëû è óïðàâëÿòü èìè.
//   2. Â îêíå Team Explorer ìîæíî ïîäêëþ÷èòüñÿ ê ñèñòåìå óïðàâëåíèÿ âåðñèÿìè.
//   3. Â îêíå "Âûõîäíûå äàííûå" ìîæíî ïðîñìàòðèâàòü âûõîäíûå äàííûå ñáîðêè è äðóãèå ñîîáùåíèÿ.
//   4. Â îêíå "Ñïèñîê îøèáîê" ìîæíî ïðîñìàòðèâàòü îøèáêè.
//   5. Ïîñëåäîâàòåëüíî âûáåðèòå ïóíêòû ìåíþ "Ïðîåêò" > "Äîáàâèòü íîâûé ýëåìåíò", ÷òîáû ñîçäàòü ôàéëû êîäà, èëè "Ïðîåêò" > "Äîáàâèòü ñóùåñòâóþùèé ýëåìåíò", ÷òîáû äîáàâèòü â ïðîåêò ñóùåñòâóþùèå ôàéëû êîäà.
//   6. ×òîáû ñíîâà îòêðûòü ýòîò ïðîåêò ïîçæå, âûáåðèòå ïóíêòû ìåíþ "Ôàéë" > "Îòêðûòü" > "Ïðîåêò" è âûáåðèòå SLN-ôàéë.
