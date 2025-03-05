// ConsoleApplication174.cpp : Ýòîò ôàéë ñîäåðæèò ôóíêöèþ "main". Çäåñü íà÷èíàåòñÿ è çàêàí÷èâàåòñÿ âûïîëíåíèå ïðîãðàììû.
//

#include <iostream>

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
    namespace Documents
    {
        struct Document
        {
            Document() {};

            //!Appended primitive
            template<typename primitive>
            void add_data(primitive data) {};
        };
    }

    /*!
    \brief Use it to decode document to bin
    \warning This class can't write information to file
    \todo Append some types of files
    */
    namespace TypeOfFiles
    {
        template<typename DocumentType>
        struct BasicCLass
        {
            std::shared_ptr<DocumentType> sh_ptr; ///< Using it to get inforamtion about file
            virtual void decode() {}; ///< This function get information from document and put this information to file
        };
    }


    /*!
    \brief Use it to write bin document to file
    \warning It don't work with GUI document
    \todo Appended some logics
    */
    namespace Writers
    {
        template<typename FileSystem, typename NameType>
        struct File
        {
            File(NameType nt) {}

            //! Put class of namespace TypeOfFiles to me
            template<typename DocumentType>
            void ImportToMe(std::shared_ptr<DocumentType> sh) {}

            //! Get class of namespace TypeOfFiles from me
            template<typename DocumentType>
            std::shared_ptr<DocumentType> ExportFromMe() {}
        };
    }

    /*!
    \brief This namespace have got some types of primitive and you can appended your primitive
    \todo Appended some primitives
    */
    namespace Primitives
    {
        class BasicPrimitive
        {
        public:
            BasicPrimitive() {}
        };
    }




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
            std::shared_ptr<DocumentType> take_document() {};

            //! Use it to put class from namespace Documents(What you include from Writers) to GUI
            void put_document(std::shared_ptr<DocumentType> dc) {};

            template<typename TypePrimitive>
            void draw_primitive(std::shared_ptr<TypePrimitive> sh) {};
        };

    }
    

    //! Examples
    //! \warning Not use it

}

int main()
{
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
